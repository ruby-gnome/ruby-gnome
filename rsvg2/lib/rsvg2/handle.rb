# Copyright (C) 2017  Ruby-GNOME2 Project Team
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

module Rsvg
  class Handle
    class << self
      # For backward compatibility
      def new_from_data(data)
        new(:data => data)
      end

      # For backward compatibility
      def new_from_file(file_name, options={})
        new(options.merge(:file => file_name))
      end
    end

    alias_method :initialize_raw, :initialize
    def initialize(options={})
      flags = options[:flags]
      path = options[:path] || options[:file]
      data = options[:data]

      if path
        if flags
          Gio::File.open(:path => path) do |file|
            initialize_new_from_gfile_sync(file, flags)
          end
        else
          initialize_new_from_file(path)
        end
      elsif data
        initialize_new_from_data(data)
      else
        initialize_raw
      end
    end
  end
end
