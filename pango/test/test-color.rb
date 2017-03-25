# Copyright (C) 2013-2017 Ruby-GNOME2 Project Team
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

class TestPangoColor < Test::Unit::TestCase
  include PangoTestUtils

  def test_red
    color = Pango::Color.new(0, 0, 0)
    assert_equal(0, color.red)
    color.red = 32768
    assert_equal(32768, color.red)
  end

  def test_blue
    color = Pango::Color.new(0, 0, 0)
    assert_equal(0, color.blue)
    color.blue = 32768
    assert_equal(32768, color.blue)
  end

  def test_green
    color = Pango::Color.new(0, 0, 0)
    assert_equal(0, color.green)
    color.green = 32768
    assert_equal(32768, color.green)
  end

  def test_to_a
    color = Pango::Color.new(65535, 32768, 0)
    assert_equal([65535, 32768, 0], color.to_a)
  end
end
