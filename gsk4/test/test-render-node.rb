# Copyright (C) 2025  Ruby-GNOME Project Team
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

require "stringio"

class TestRenderNode < Test::Unit::TestCase
  def setup
    width = 1
    height = 1
    rgba = Gdk::RGBA.new(1.0, 0.0, 0.0, 1.0)
    bounds = Graphene::Rect.new(0, 0, width, height)
    @node = Gsk::ColorNode.new(rgba, bounds)
    @surface = Cairo::ImageSurface.new(:ARGB32, width, height)
    @context = Cairo::Context.new(@surface)
  end

  def test_draw
    @node.draw(@context)
    argbs = @surface.data.unpack("L*").collect do |argb|
      # [alpha (8 bits), red (8 bits), green (8 bits), blue (8 bits)]
      lower_8_bits_mask = (1 << 8) - 1
      [
        argb >> 24,
        (argb >> 16) & lower_8_bits_mask,
        (argb >> 8) & lower_8_bits_mask,
        argb & lower_8_bits_mask,
      ]
    end
    assert_equal([[255, 255, 0, 0]],
                 argbs)
  end
end
