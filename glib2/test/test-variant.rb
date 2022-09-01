# Copyright (C) 2018-2022  Ruby-GNOME Project Team
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

  sub_test_case ".parse" do
    test "no type" do
      variant = GLib::Variant.parse("'hello'")
      assert_equal([GLib::VariantType::STRING, "hello"],
                   [variant.type, variant.value])
    end

    test "type" do
      variant = GLib::Variant.parse("29", "y")
      assert_equal([GLib::VariantType::BYTE, 29],
                   [variant.type, variant.value])
    end

    test "garbage" do
      message = <<-MESSAGE
expected end of input:
  29 hello
     ^    
      MESSAGE
      assert_raise(GLib::VariantParseError::InputNotAtEnd.new(message)) do
        GLib::Variant.parse("29 hello", "y")
      end
    end
  end

  sub_test_case "#initialize" do
    test "type: string" do
      variant = GLib::Variant.new("hello", "s")
      assert_equal([GLib::VariantType::STRING, "hello"],
                   [variant.type, variant.value])
    end
  end

  sub_test_case "#to_s" do
    test "default" do
      variant = GLib::Variant.new(29, "y")
      assert_equal("0x1d", variant.to_s)
    end

    test "type_annotate:" do
      variant = GLib::Variant.new(29, "y")
      assert_equal("byte 0x1d", variant.to_s(type_annotate: true))
    end
  end

  test "#inspect" do
    variant = GLib::Variant.new(29, "y")
    assert_equal("#<GLib::Variant:0x00 type=y value=29>",
                 variant.inspect.gsub(/0x\h+/, "0x00"))
  end
end
