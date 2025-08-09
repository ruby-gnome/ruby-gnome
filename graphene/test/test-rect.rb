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

class TestRect < Test::Unit::TestCase
  sub_test_case("#initialize") do
    test("nothing") do
      rect = Graphene::Rect.new
      assert_equal([0.0, 0.0, 0.0, 0.0], rect.to_a)
    end

    test("rectangle") do
      source_rect = Graphene::Rect.new(1.0, 2.0, 3.0, 4.0)
      rect = Graphene::Rect.new(source_rect)
      assert_equal([1.0, 2.0, 3.0, 4.0], rect.to_a)
    end

    test("x, y, width, height") do
      rect = Graphene::Rect.new(1.0, 2.0, 3.0, 4.0)
      assert_equal([1.0, 2.0, 3.0, 4.0], rect.to_a)
    end
  end
end
