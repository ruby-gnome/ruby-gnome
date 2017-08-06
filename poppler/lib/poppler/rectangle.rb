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

module Poppler
  class Rectangle
    alias_method :initialize_raw, :initialize
    def initialize(*args)
      super()
      return if args.empty?

      self.x1 = args[0]
      self.y1 = args[1]
      self.x2 = args[2]
      self.y2 = args[3]
    end

    def to_a
      [x1, y1, x2, y2]
    end

    def inspect
      super.gsub(/>\z/) do
        ": [%g, %g, %g, %g]>" % to_a
      end
    end
  end
end
