# Copyright (C) 2012  Ruby-GNOME2 Project Team
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
  class Repository
    class << self
      alias_method :default_raw, :default
      def default
        @@default ||= default_raw
      end
    end

    include Enumerable

    def each(*namespaces)
      if namespaces.empty?
        namespaces = loaded_namespaces
      end
      namespaces.each do |namespace|
        get_n_infos(namespace).times do |i|
          yield(get_info(namespace, i))
        end
      end
    end
  end
end
