# -*- coding: utf-8 -*-
#
# Copyright (C) 2013-2014  Ruby-GNOME2 Project Team
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

class TestGdkRGBA < Test::Unit::TestCase
  def test_to_s
    rgba = Gdk::RGBA.new(0.2, 0.4, 0.6, 0.5)
    assert_equal("rgba(51,102,153,0.5)", rgba.to_s)
  end

  def test_to_a
    rgba = Gdk::RGBA.new(0.2, 0.4, 0.6, 0.5)
    assert_equal([0.2, 0.4, 0.6, 0.5], rgba.to_a)
  end

  sub_test_case("new") do
    def test_empty
      rgba = Gdk::RGBA.new
      assert_equal([0.0, 0.0, 0.0, 1.0], rgba.to_a)
    end

    def test_rgb
      rgba = Gdk::RGBA.new(0.2, 0.4, 0.6)
      assert_equal([0.2, 0.4, 0.6, 1.0], rgba.to_a)
    end
  end
end
