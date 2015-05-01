# -*- coding: utf-8 -*-
#
# Copyright (C) 2013-2015  Ruby-GNOME2 Project Team
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

class TestGdkRectangle < Test::Unit::TestCase
  def test_intersect
    rectangle1 = Gdk::Rectangle.new(0, 10, 20, 30)
    rectangle2 = Gdk::Rectangle.new(5, 15, 10, 20)
    intersected = rectangle1.intersect(rectangle2)
    assert_equal([
                   5,
                   15,
                   10,
                   20,
                 ],
                 [
                   intersected.x,
                   intersected.y,
                   intersected.width,
                   intersected.height,
                 ])
  end

  def test_union
    rectangle1 = Gdk::Rectangle.new(0, 10, 20, 30)
    rectangle2 = Gdk::Rectangle.new(5, 15, 40, 20)
    unioned_rectangle = rectangle1.union(rectangle2)
    assert_equal([
                   0,
                   10,
                   45,
                   30,
                 ],
                 [
                   unioned_rectangle.x,
                   unioned_rectangle.y,
                   unioned_rectangle.width,
                   unioned_rectangle.height,
                 ])
  end

  def test_to_a
    rectangle = Gdk::Rectangle.new(0, 10, 20, 30)
    assert_equal([
                   0,
                   10,
                   20,
                   30,
                 ],
                 rectangle.to_a)
  end

  test "#==" do
    rectangle1 = Gdk::Rectangle.new(0, 10, 20, 30)
    rectangle2 = Gdk::Rectangle.new(0, 10, 20, 30)
    assert do
      rectangle1 == rectangle2
    end
  end
end
