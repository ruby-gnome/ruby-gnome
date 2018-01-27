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

module Gdk
  class Rectangle
    alias_method :initialize_raw, :initialize
    def initialize(x, y, width, height)
      initialize_raw
      self.x = x
      self.y = y
      self.width = width
      self.height = height
    end

    alias_method :intersect_raw, :intersect
    def intersect(other)
      intersected, dest = intersect_raw(other)
      if intersected
        dest
      else
        nil
      end
    end

    def inspect
      super.gsub(/>\z/) do
        " x=#{x.inspect} y=#{y.inspect} " +
          "width=#{width.inspect} height=#{height.inspect}>"
      end
    end

    def to_a
      [x, y, width, height]
    end

    unless instance_method(:==).owner == self
      def ==(other)
        other.is_a?(self.class) and
          self.to_a == other.to_a
      end
    end
  end
end
