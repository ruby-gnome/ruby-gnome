# Copyright (C) 2016-2018  Ruby-GNOME2 Project Team
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
        if args[0].is_a?(String)
          initialize_raw(args[0])
        else
          initialize_with_hash_args(args[0])
        end
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
        message =
          "must specify one of the following combinations: " +
          "schema_id, " +
          "options, " +
          "schema_id and options, " +
          "schema and backend and path " +
          "instead of: #{args.inspect}"
        raise ArgumentError, message
      end
    end

    def initialize_with_hash_args(options)
      path = options[:path] || nil
      schema_id = options[:schema_id] || nil
      backend = options[:backend] || nil
      schema = options[:schema] || nil
      if schema_id
        if path && backend
          initialize_new_with_backend_and_path(schema_id, backend, path)
        elsif path
          initialize_new_with_path(schema_id, path)
        elsif backend
          initialize_new_with_backend(schema_id, backend)
        else
          initialize_raw(schema_id)
        end
      elsif schema
        initialize_new_full(schema, backend, path)
      else
        message =
          "must specify one of the following combinations: " +
          ":schema_id, " +
          ":schema_id and :path, " +
          ":schema_id and :backend, " +
          ":schema_id and :backend and :path, " +
          ":schema, " +
          ":schema and :path, " +
          ":schema and :backend, " +
          "or :schema and :backend and :path " +
          "instead of: #{options.inspect}"
        raise ArgumentError, message
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
