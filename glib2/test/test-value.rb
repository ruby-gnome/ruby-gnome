# Copyright (C) 2013-2025  Ruby-GNOME Project Team
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

class TestGLibValue < Test::Unit::TestCase
  include GLibTestUtils

  sub_test_case(".try_convert") do
    def test_string
      assert_equal(GLib::Type::STRING,
                   GLib::Value.try_convert("Hello").type)
    end

    def test_integer_negative_small
      assert_equal(GLib::Type::INT,
                   GLib::Value.try_convert(-(2 ** 31)).type)
    end

    def test_integer_negative_large
      assert_equal(GLib::Type::INT64,
                   GLib::Value.try_convert(-(2 ** 31 + 1)).type)
    end

    def test_integer_zero
      assert_equal(GLib::Type::UINT,
                   GLib::Value.try_convert(0).type)
    end

    def test_integer_positive_small
      assert_equal(GLib::Type::UINT,
                   GLib::Value.try_convert(2 ** 32 - 1).type)
    end

    def test_integer_positive_large
      assert_equal(GLib::Type::UINT64,
                   GLib::Value.try_convert(2 ** 32).type)
    end

    def test_float
      assert_equal(GLib::Type::DOUBLE,
                   GLib::Value.try_convert(-0.0).type)
    end
  end

  def test_type
    value = GLib::Value.new(GLib::Type::UINT, 29)
    assert_equal(GLib::Type::UINT, value.type)
  end

  def test_value
    value = GLib::Value.new(GLib::Type::UINT, 29)
    assert_equal(29, value.value)
  end

  def test_no_value
    value = GLib::Value.new(GLib::Type::UINT)
    assert_equal(0, value.value)
  end
end
