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

class ClutterColorTest < Test::Unit::TestCase
  def test_rgb
    assert_equal("#ff0000ff", Clutter::Color.rgb(255, 0, 0).to_s)
  end

  def test_rgba
    assert_equal("#ff00007f", Clutter::Color.rgb(255, 0, 0, 127).to_s)
  end

  def test_hls
    assert_equal("#20dfdfff", Clutter::Color.hls(180, 0.5, 0.75).to_s)
  end

  def test_hlsa
    assert_equal("#20dfdf7f", Clutter::Color.hls(180, 0.5, 0.75, 127).to_s)
  end

  def test_pixel
    assert_equal("#20dfdf7f", Clutter::Color.pixel(0x20dfdf7f).to_s)
  end

  def test_parse
    assert_equal("#20dfdf7f",
                 Clutter::Color.parse("rgba(32, 223, 223, 0.5)").to_s)
  end
end
