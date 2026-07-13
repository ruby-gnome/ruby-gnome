# Copyright (C) 2012-2016  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

module GObjectIntrospection
  module CollectionReader
    def collection_reader(name)
      n_getter = "n_#{name}"
      if name.end_with?("ies")
        singular = name.sub(/ies\z/, "y")
      else
        singular = name.sub(/s\z/, "")
      end
      getter = "get_#{singular}"
      cache_name = "@#{name}"
      define_method(name) do
        if instance_variable_defined?(cache_name)
          instance_variable_get(cache_name)
        else
          collection = send(n_getter).times.collect do |i|
            send(getter, i)
          end
          instance_variable_set(cache_name, collection)
          collection
        end
      end
    end
  end
end
