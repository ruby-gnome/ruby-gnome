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

class TestRoundedRect < Test::Unit::TestCase
  sub_test_case(".try_convert") do
    test("[[x, y, w, h], radius]") do
      rect = Gsk::RoundedRect.try_convert([[1.0, 2.0, 11.0, 12.0], 3.0])
      assert_equal([
                     [1.0, 2.0, 11.0, 12.0],
                     [
                       [3.0, 3.0],
                       [3.0, 3.0],
                       [3.0, 3.0],
                       [3.0, 3.0],
                     ],
                   ],
                   [rect.bounds.to_a, rect.corners.collect(&:to_a)])
    end

    test("[[x, y, w, h], top_left, top_right, bottom_right, bottom_left]") do
      rect = Gsk::RoundedRect.try_convert([[1.0, 2.0, 11.0, 12.0],
                                           [1.0, 2.0],
                                           [2.0, 3.0],
                                           [3.0, 4.0],
                                           [4.0, 5.0]])
      assert_equal([
                     [1.0, 2.0, 11.0, 12.0],
                     [
                       [1.0, 2.0],
                       [2.0, 3.0],
                       [3.0, 4.0],
                       [4.0, 5.0],
                     ],
                   ],
                   [rect.bounds.to_a, rect.corners.collect(&:to_a)])
    end

    test("Array: invalid") do
      assert_nil(Gsk::RoundedRect.try_convert([1.0]))
    end
  end

  sub_test_case("#initialize") do
    test("nothing") do
      rect = Gsk::RoundedRect.new
      assert_equal([
                     [0.0, 0.0, 0.0, 0.0],
                     [
                       [0.0, 0.0],
                       [0.0, 0.0],
                       [0.0, 0.0],
                       [0.0, 0.0],
                     ],
                   ],
                   [rect.bounds.to_a, rect.corners.collect(&:to_a)])
    end

    test("RoundedRect") do
      source_rect = Gsk::RoundedRect.new([1.0, 2.0, 11.0, 12.0], 3.0)
      rect = Gsk::RoundedRect.new(source_rect)
      assert_equal(source_rect, rect)
    end

    test("Rect, radius") do
      rect = Gsk::RoundedRect.new([1.0, 2.0, 11.0, 12.0], 3.0)
      assert_equal([
                     [1.0, 2.0, 11.0, 12.0],
                     [
                       [3.0, 3.0],
                       [3.0, 3.0],
                       [3.0, 3.0],
                       [3.0, 3.0],
                     ],
                   ],
                   [rect.bounds.to_a, rect.corners.collect(&:to_a)])
    end

    test("bounds, top_left, top_right, bottom_right, bottom_left") do
      rect = Gsk::RoundedRect.new([1.0, 2.0, 11.0, 12.0],
                                  [1.0, 2.0],
                                  [2.0, 3.0],
                                  [3.0, 4.0],
                                  [4.0, 5.0])
      assert_equal([
                     [1.0, 2.0, 11.0, 12.0],
                     [
                       [1.0, 2.0],
                       [2.0, 3.0],
                       [3.0, 4.0],
                       [4.0, 5.0],
                     ],
                   ],
                   [rect.bounds.to_a, rect.corners.collect(&:to_a)])
    end
  end
end
