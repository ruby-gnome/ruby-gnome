# Copyright (C) 2018  Ruby-GNOME2 Project Team
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

class TestGLibVariant < Test::Unit::TestCase
  include GLibTestUtils

  sub_test_case "#initialize" do
    test "type: string" do
      variant = GLib::Variant.new("hello", "s")
      assert_equal([GLib::VariantType::STRING, "hello"],
                   [variant.type, variant.value])
    end
  end

  sub_test_case ".parse" do
    test "value: true" do
      variant = GLib::Variant.parse("true")
      assert_equal(true, variant.value)
    end

    test "value: float" do
      variant = GLib::Variant.parse(Math::PI.to_s)
      assert_equal(Math::PI, variant.value)
    end

    test "value: infinity" do
      variant = GLib::Variant.parse("inf")
      assert_equal(Float::INFINITY, variant.value)
    end

    test "value: int vector" do
      variant = GLib::Variant.parse("[1, 2, 3]")
      assert_equal([1, 2, 3], variant.value)
    end

    test "value: integer, type: UINT16" do
      variant = GLib::Variant.parse("65535", GLib::VariantType::UINT16)
      assert_equal(65535, variant.value)
      assert_equal("q", variant.type.to_s)
    end
  end

  sub_test_case "#variant_print" do
    test "value: true" do
      variant = GLib::Variant.new(true)
      out = variant.variant_print
      assert_equal("true", out)
    end

    test "value: float" do
      variant = GLib::Variant.new(Math::PI)
      out = variant.variant_print
      ## this may avoid mismatch from rounding error
      assert_equal(Math::PI, out.to_f)
    end

    test "value: infinity" do
      variant = GLib::Variant.new(Float::INFINITY)
      out = variant.variant_print
      assert_equal("inf", out)
    end

    test "value: int vector" do
      variant = GLib::Variant.new([1, 2, 3])
      out = variant.variant_print
      assert_equal("[1, 2, 3]", out)
    end

    test "value: integer, type: UINT16, type_annotate: true" do
      variant = GLib::Variant.new(65535, GLib::VariantType::UINT16)
      out = variant.variant_print(true)
      assert_equal("uint16 65535", out)
    end

    test "value: int vector, type_anntoate: true" do
      variant = GLib::Variant.new([1, 2, 3])
      out = variant.variant_print(true)
      assert_equal("[int64 1, 2, 3]", out)
    end
  end
end
