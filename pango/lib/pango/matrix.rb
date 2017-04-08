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

module Pango
  class Matrix
    alias_method :initialize_raw, :initialize
    def initialize(xx=nil, xy=nil, yx=nil, yy=nil, x0=nil, y0=nil)
      initialize_raw
      self.xx = xx || 1.0
      self.xy = xy || 0.0
      self.yx = yx || 0.0
      self.yy = yy || 1.0
      self.x0 = x0 || 0.0
      self.y0 = y0 || 0.0
    end

    def to_a
      [
        xx, xy,
        yx, yy,
        x0,
        y0,
      ]
    end

    def translate(tx, ty)
      copied_matrix = dup
      copied_matrix.translate!(tx, ty)
      copied_matrix
    end

    def scale(scale_x, scale_y)
      copied_matrix = dup
      copied_matrix.scale!(scale_x, scale_y)
      copied_matrix
    end

    def rotate(degree)
      copied_matrix = dup
      copied_matrix.rotate!(degree)
      copied_matrix
    end

    def concat(matrix)
      copied_matrix = dup
      copied_matrix.concat!(matrix)
      copied_matrix
    end
    alias_method :+, :concat
  end
end
