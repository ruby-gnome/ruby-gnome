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

module Gdk
  class Cursor
    @caches = {}
    class << self
      def new(*args)
        first_arg = args.first
        case first_arg
        when String, Symbol, CursorType
          @caches[first_arg] ||= super
        else
          super
        end
      end
    end

    alias_method :initialize_raw, :initialize
    def initialize(*args)
      if args.last.is_a?(Hash)
        options = args.pop
      else
        options = {}
      end
      display = options[:display] || Display.default

      if args.size == 1 and args[0].is_a?(String)
        initialize_new_from_name(display, *args)
      else
        initialize_raw(display, *args)
      end
    end
  end
end
