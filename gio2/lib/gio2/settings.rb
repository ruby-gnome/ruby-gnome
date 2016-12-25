# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

module Gio
  class Settings
    alias_method :initialize_raw, :initialize

    def initialize(*args)
      if args.size == 1
        initialize_raw(args[0])
      elsif args.size == 2
        schema_id = args[0]
        options = args[1]
        path = options[:path] || nil
        backend = options[:backend] || nil
        if path && backend
          initialize_new_with_backend_and_path(schema_id, backend, path)
        elsif path
          initialize_new_with_path(schema_id, path)
        elsif backend
          initialize_new_with_backend(schema_id, backend)
        end
      elsif args.size == 3
        initialize_new_full(*args)
      else
        $stderr.puts "Arguments error for Gio::Settings#new"
      end
    end

    alias_method :set_value_raw, :set_value
    def set_value(key, value)
      schema_key = settings_schema.get_key(key)
      variant_value = GLib::Variant.new(value, schema_key.value_type)
      set_value_raw(key, variant_value)
    end

    alias_method :[], :get_value
    alias_method :[]=, :set_value
  end
end
