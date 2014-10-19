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

class ClutterCanvasTest < Test::Unit::TestCase
  include ClutterTestUtils

  def setup
    @canvas = Clutter::Canvas.new
  end

  def test_scale_factor
    only_clutter_version(1, 18, 0)
    scale = 2
    @canvas.scale_factor = scale
    assert_equal(scale, @canvas.scale_factor)
    assert_true(@canvas.scale_factor_set?)
  end

  def test_height_accessors
    height = 320
    @canvas.height = height
    assert_equal(height, @canvas.height)
  end

  def test_width_accessors
    width = 640
    @canvas.width = width
    assert_equal(width, @canvas.width)
  end
end
