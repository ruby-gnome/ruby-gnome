# Copyright (C) 2016-2017  Ruby-GNOME2 Project Team
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
  module File
    class << self
      def open(options={})
        arg = options[:arg]
        cwd = options[:cwd]
        path = options[:path]
        uri = options[:uri]

        if arg
          if cwd
            file = new_for_commandline_arg_and_cmd(arg, cwd)
          else
            file = new_for_commandline_arg(arg)
          end
        elsif path
          file = new_for_path(path)
        elsif uri
          file = new_for_uri(uri)
        else
          message = "must specify :arg, :path or :uri: #{options.inspect}"
          raise ArgumentError, message
        end

        if block_given?
          yield(file)
        else
          file
        end
      end
    end
  end
end
