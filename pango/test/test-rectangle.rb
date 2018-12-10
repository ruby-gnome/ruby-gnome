# Copyright (C) 2013-2018  Ruby-GNOME2 Project Team
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

class TestPangoRectangle < Test::Unit::TestCase
  include PangoTestUtils

  def test_x
    rectangle = Pango::Rectangle.new(0, 10, 20, 30)
    assert_equal(0, rectangle.x)
  end

  def test_y
    rectangle = Pango::Rectangle.new(0, 10, 20, 30)
    assert_equal(10, rectangle.y)
  end

  def test_width
    rectangle = Pango::Rectangle.new(0, 10, 20, 30)
    assert_equal(20, rectangle.width)
  end

  def test_height
    rectangle = Pango::Rectangle.new(0, 10, 20, 30)
    assert_equal(30, rectangle.height)
  end

  def test_ascent
    rectangle = Pango::Rectangle.new(5, 10, 20, 30)
    assert_equal(-10, rectangle.ascent)
  end

  def test_descent
    rectangle = Pango::Rectangle.new(5, 10, 20, 30)
    assert_equal(40, rectangle.descent)
  end

  def test_left_bearing
    rectangle = Pango::Rectangle.new(5, 10, 20, 30)
    assert_equal(5, rectangle.left_bearing)
  end

  def test_right_bearing
    rectangle = Pango::Rectangle.new(5, 10, 20, 30)
    assert_equal(25, rectangle.right_bearing)
  end
end
