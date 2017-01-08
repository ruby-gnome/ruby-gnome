# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

module Gst
  class Caps
    class << self
      def any
        new(:any)
      end

      def empty
        new(:empty)
      end
    end

    alias_method :initialize_raw, :initialize
    def initialize(*args)
      if args.size == 1
        case args[0]
        when :any
          initialize_new_any
          return
        when :empty
          initialize_new_empty
          return
        end
      end
      initialize_raw(*args)
    end

    def structures
      size.times.collect do |i|
        get_structure(i)
      end
    end
  end
end
