# Copyright (C) 2014-2015  Ruby-GNOME2 Project Team
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
      data = options[:data]
      file = options[:file]
      path = options[:path]
      resource_path = options[:resource_path]
      if data
        load_from_data(data)
      elsif file
        load_from_file(file)
      elsif path
        load_from_path(path)
      elsif resource_path
        load_from_resource(resource_path)
      else
        message = "Must specify one of :data, :file, :path or :resource_path"
        raise ArgumentError, "#{message}: #{options.inspect}"
      end
    end

    alias_method :load_from_data_raw, :load_from_data
    def load_from_data(data)
      if data.is_a?(GLib::Bytes)
        load_from_data_raw(data.to_s)
      else
        load_from_data_raw(data)
      end
    end
  end
end
