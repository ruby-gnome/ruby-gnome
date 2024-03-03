# Copyright (C) 2014-2024  Ruby-GNOME Project Team
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
  class CssProvider
    def load(options)
      string = options[:string]
      bytes = options[:bytes]
      data = options[:data]
      file = options[:file]
      path = options[:path]
      resource_path = options[:resource_path]
      if string
        if Version.or_later?(4, 12, 0)
          load_from_string(string)
        else
          load_from_data(string)
        end
      elsif bytes
        if Version.or_later?(4, 12, 0)
          load_from_bytes(bytes)
        else
          load_from_data(bytes)
        end
      elsif data
        if Version.or_later?(4, 12, 0)
          load_from_bytes(data)
        else
          load_from_data(data)
        end
      elsif file
        load_from_file(file)
      elsif path
        load_from_path(path)
      elsif resource_path
        load_from_resource(resource_path)
      else
        message = "Must specify one of " +
                  ":string, :bytes, :data, :file, :path or :resource_path"
        raise ArgumentError, "#{message}: #{options.inspect}"
      end
    end

    alias_method :load_from_data_raw, :load_from_data
    if Version.or_later?(4, 9, 3)
      def load_from_data(data)
        data = data.to_s if data.is_a?(GLib::Bytes)
        load_from_data_raw(data, data.bytesize)
      end
    else
      def load_from_data(data)
        data = data.to_s if data.is_a?(GLib::Bytes)
        load_from_data_raw(data)
      end
    end
  end
end
