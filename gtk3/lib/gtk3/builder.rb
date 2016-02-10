# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

module Gtk
  class Builder
    alias_method :initialize_raw, :initialize
    def initialize(options={})
      path      = options[:path] || options[:file]
      resource  = options[:resource]
      string    = options[:string]

      if path
        path = path.to_path if path.respond_to?(:to_path)
        initialize_new_from_file(path)
      elsif resource
        initialize_new_from_resource(resource)
      elsif string
        initialize_new_from_string(string, string.bytesize)
      else
        initialize_raw
      end
    end

    alias_method :[], :get_object

    alias_method :add_from_string_raw, :add_from_string
    def add_from_string(string)
      add_from_string_raw(string, string.bytesize)
    end

    alias_method :add_objects_from_string_raw, :add_objects_from_string
    def add_objects_from_string(string, object_ids)
      add_objects_from_string_raw(string, string.bytesize, object_ids)
    end

    def add(target_or_options={})
      if target_or_options.is_a?(Hash)
        options = target_or_options
      else
        target = target_or_options
        options = {}
        if target.respond_to?(:to_path)
          options[:path] = target.to_path
        elsif target.start_with?("<") or target.start_with?(" ")
          options[:string] = target
        elsif File.exist?(target)
          options[:path] = target
        else
          options[:resource] = target
        end
      end

      string   = options[:string]
      path     = options[:path] || options[:file]
      resource = options[:resource]

      object_ids = options[:object_ids]

      if path
        path = path.to_path if path.respond_to?(:to_path)
        if object_ids
          add_objects_from_file(path, object_ids)
        else
          add_from_file(path)
        end
      elsif resource
        if object_ids
          add_objects_from_resource(resource, object_ids)
        else
          add_from_resource(resource)
        end
      elsif string
        if object_ids
          add_objects_from_string(string, object_ids)
        else
          add_from_string(string)
        end
      else
        message = ":path (:file), :resource or :string " +
          "must be specified: #{options.inspect}"
        raise InvalidArgument, message
      end
    end

    def <<(target)
      add(target)
      self
    end
  end
end
