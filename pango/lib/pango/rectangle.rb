# Copyright (C) 2017-2018  Ruby-GNOME2 Project Team
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

module Pango
  class Rectangle
    alias_method :initialize_raw, :initialize
    def initialize(x, y, width, height)
      initialize_raw
      self.x = x
      self.y = y
      self.width = width
      self.height = height
    end

    def ascent
      -y
    end

    def descent
      y + height
    end

    def left_bearing
      x
    end

    def right_bearing
      x + width
    end

    def to_a
      [x, y, width, height]
    end

    def dup
      duplicated = self.class.new(x, y, width, height)
      duplicated.taint if tainted?
      duplicated
    end
  end
end
