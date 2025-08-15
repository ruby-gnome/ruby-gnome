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

class TestPoint < Test::Unit::TestCase
  sub_test_case(".try_convert") do
    test("[x, y]") do
      point = Graphene::Point.try_convert([1.0, 2.0])
      assert_equal([1.0, 2.0], point.to_a)
    end

    test("Array: invalid") do
      assert_nil(Graphene::Point.try_convert([1.0]))
    end
  end

  sub_test_case("#initialize") do
    test("nothing") do
      point = Graphene::Point.new
      assert_equal([0.0, 0.0], point.to_a)
    end

    test("Graphene::Point") do
      source_point = Graphene::Point.new(1.0, 2.0)
      point = Graphene::Point.new(source_point)
      assert_equal([1.0, 2.0], point.to_a)
    end

    test("Graphene::Vec2") do
      source_vector = Graphene::Vec2.new(1.0, 2.0)
      point = Graphene::Point.new(source_vector)
      assert_equal([1.0, 2.0], point.to_a)
    end

    test("x, y") do
      point = Graphene::Point.new(1.0, 2.0)
      assert_equal([1.0, 2.0], point.to_a)
    end
  end
end
