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

class TestCairoScaledFont < Test::Unit::TestCase
  def setup
    surface = Cairo::ImageSurface.new(Cairo::Format::ARGB32, 290, 200)
    context = Cairo::Context.new(surface)
    @font_face = context.font_face
    @font_matrix = Cairo::Matrix.identity
    @scale = Cairo::Matrix.scale(0.5, 1.0)
    @font_option = Cairo::FontOptions.new
  end

  def test_cairo_scaled_font
    assert_nothing_raised do
      Cairo::ScaledFont.new(@font_face, @font_matrix, @scale, @font_option)
    end
  end
end
