# Copyright (C) 2014  Ruby-GNOME2 Project Team
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
  module Resources
    class << self
      unless method_defined?(:register)
        def register(resource)
          resource._register
        end
      end

      unless method_defined?(:unregister)
        def unregister(resource)
          resource._unregister
        end
      end

      alias_method :lookup_data_raw, :lookup_data
      def lookup_data(path, flags=nil)
        flags ||= ResourceLookupFlags::NONE
        lookup_data_raw(path, flags)
      end

      alias_method :open_stream_raw, :open_stream
      def open_stream(path, flags=nil)
        flags ||= ResourceLookupFlags::NONE
        input_stream = open_stream_raw(path, flags)
        if block_given?
          begin
            yield(input_stream)
          ensure
            input_stream.close
          end
        else
          input_stream
        end
      end

      alias_method :enumerate_children_raw, :enumerate_children
      def enumerate_children(path, flags=nil)
        flags ||= ResourceLookupFlags::NONE
        enumerate_children_raw(path, flags)
      end

      alias_method :get_info_raw, :get_info
      def get_info(path, flags=nil)
        flags ||= ResourceLookupFlags::NONE
        get_info_raw(path, flags)
      end
    end
  end
end
