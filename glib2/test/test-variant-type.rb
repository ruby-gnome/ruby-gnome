# Copyright (C) 2015-2021  Ruby-GNOME Project Team
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

class TestGLibVariantType < Test::Unit::TestCase
  include GLibTestUtils

  sub_test_case "constants" do
    test "BOOLEAN" do
      assert_equal("b", GLib::VariantType::BOOLEAN.to_s)
    end

    test "BYTE" do
      assert_equal("y", GLib::VariantType::BYTE.to_s)
    end

    test "INT16" do
      assert_equal("n", GLib::VariantType::INT16.to_s)
    end

    test "UINT16" do
      assert_equal("q", GLib::VariantType::UINT16.to_s)
    end

    test "INT32" do
      assert_equal("i", GLib::VariantType::INT32.to_s)
    end

    test "UINT32" do
      assert_equal("u", GLib::VariantType::UINT32.to_s)
    end

    test "INT64" do
      assert_equal("x", GLib::VariantType::INT64.to_s)
    end

    test "UINT64" do
      assert_equal("t", GLib::VariantType::UINT64.to_s)
    end

    test "HANDLE" do
      assert_equal("h", GLib::VariantType::HANDLE.to_s)
    end

    test "DOUBLE" do
      assert_equal("d", GLib::VariantType::DOUBLE.to_s)
    end

    test "STRING" do
      assert_equal("s", GLib::VariantType::STRING.to_s)
    end

    test "OBJECT_PATH" do
      assert_equal("o", GLib::VariantType::OBJECT_PATH.to_s)
    end

    test "SIGNATURE" do
      assert_equal("g", GLib::VariantType::SIGNATURE.to_s)
    end

    test "VARIANT" do
      assert_equal("v", GLib::VariantType::VARIANT.to_s)
    end

    test "ANY" do
      assert_equal("*", GLib::VariantType::ANY.to_s)
    end

    test "BASIC" do
      assert_equal("?", GLib::VariantType::BASIC.to_s)
    end

    test "MAYBE" do
      assert_equal("m*", GLib::VariantType::MAYBE.to_s)
    end

    test "ARRAY" do
      assert_equal("a*", GLib::VariantType::ARRAY.to_s)
    end

    test "TUPLE" do
      assert_equal("r", GLib::VariantType::TUPLE.to_s)
    end

    test "UNIT" do
      assert_equal("()", GLib::VariantType::UNIT.to_s)
    end

    test "DICT_ENTRY" do
      assert_equal("{?*}", GLib::VariantType::DICT_ENTRY.to_s)
    end

    test "DICTIONARY" do
      assert_equal("a{?*}", GLib::VariantType::DICTIONARY.to_s)
    end

    test "STRING_ARRAY" do
      assert_equal("as", GLib::VariantType::STRING_ARRAY.to_s)
    end

    test "OBJECT_PATH_ARRAY" do
      assert_equal("ao", GLib::VariantType::OBJECT_PATH_ARRAY.to_s)
    end

    test "BYTESTRING" do
      assert_equal("ay", GLib::VariantType::BYTESTRING.to_s)
    end

    test "BYTESTRING_ARRAY" do
      assert_equal("aay", GLib::VariantType::BYTESTRING_ARRAY.to_s)
    end

    test "VARDICT" do
      assert_equal("a{sv}", GLib::VariantType::VARDICT.to_s)
    end
  end

  sub_test_case ".valid?" do
    test "valid" do
      assert do
        GLib::VariantType.valid?("s")
      end
    end

    test "invalid" do
      assert do
        !GLib::VariantType.valid?("X")
      end
    end
  end

  sub_test_case ".scan" do
    test "found" do
      assert_equal("X", GLib::VariantType.scan("sX"))
    end

    test "not found" do
      assert_nil(GLib::VariantType.scan("X"))
    end
  end

  sub_test_case "#initialize" do
    test "invalid" do
      assert_raise(ArgumentError.new("invalid type string: \"X\"")) do
        GLib::VariantType.new("X")
      end
    end
  end

  test "#to_s" do
    variant_type = GLib::VariantType.new("s")
    assert_equal("s", variant_type.to_s)
  end

  sub_test_case "#definite?" do
    test "definite" do
      variant_type = GLib::VariantType.new("s")
      assert do
        variant_type.definite?
      end
    end

    test "not definite" do
      variant_type = GLib::VariantType.new("?")
      assert do
        !variant_type.definite?
      end
    end
  end

  sub_test_case "#container?" do
    test "container" do
      variant_type = GLib::VariantType.new("a*")
      assert do
        variant_type.container?
      end
    end

    test "not container" do
      variant_type = GLib::VariantType.new("s")
      assert do
        !variant_type.container?
      end
    end
  end

  sub_test_case "#basic?" do
    test "basic" do
      variant_type = GLib::VariantType.new("s")
      assert do
        variant_type.basic?
      end
    end

    test "not basic" do
      variant_type = GLib::VariantType.new("as")
      assert do
        !variant_type.basic?
      end
    end
  end

  sub_test_case "#maybe?" do
    test "maybe" do
      variant_type = GLib::VariantType.new("ms")
      assert do
        variant_type.maybe?
      end
    end

    test "not maybe" do
      variant_type = GLib::VariantType.new("s")
      assert do
        !variant_type.maybe?
      end
    end
  end

  sub_test_case "#array?" do
    test "array" do
      variant_type = GLib::VariantType.new("as")
      assert do
        variant_type.array?
      end
    end

    test "not array" do
      variant_type = GLib::VariantType.new("s")
      assert do
        !variant_type.array?
      end
    end
  end

  sub_test_case "#tuple?" do
    test "tuple" do
      variant_type = GLib::VariantType.new("(s)")
      assert do
        variant_type.tuple?
      end
    end

    test "not tuple" do
      variant_type = GLib::VariantType.new("s")
      assert do
        !variant_type.tuple?
      end
    end
  end

  sub_test_case "#dict_entry?" do
    test "dictionary entry" do
      variant_type = GLib::VariantType.new("{ss}")
      assert do
        variant_type.dict_entry?
      end
    end

    test "not dictionary entry" do
      variant_type = GLib::VariantType.new("s")
      assert do
        !variant_type.dict_entry?
      end
    end
  end

  sub_test_case "#variant?" do
    test "variant" do
      variant_type = GLib::VariantType.new("v")
      assert do
        variant_type.variant?
      end
    end

    test "not variant" do
      variant_type = GLib::VariantType.new("s")
      assert do
        !variant_type.variant?
      end
    end
  end

  sub_test_case "#==" do
    test "same" do
      variant_type = GLib::VariantType.new("s")
      assert_equal(variant_type,
                   variant_type)
    end

    test "equivalence" do
      assert_equal(GLib::VariantType.new("s"),
                   GLib::VariantType.new("s"))
    end

    test "not VariantType" do
      assert_not_equal(GLib::VariantType.new("s"),
                       "s")
    end
  end

  sub_test_case "#hash and #eql?" do
    test "Hash key" do
      hash = {
        GLib::VariantType.new("s") => :s,
      }
      assert_equal(:s, hash[GLib::VariantType.new("s")])
    end
  end

  sub_test_case "#is_subtype_of?" do
    test "subtype" do
      string = GLib::VariantType.new("s")
      basic = GLib::VariantType.new("?")
      assert do
        string.is_subtype_of?(basic)
      end
    end

    test "not subtype" do
      string = GLib::VariantType.new("s")
      int32 = GLib::VariantType.new("i")
      assert do
        !string.is_subtype_of?(int32)
      end
    end
  end

  sub_test_case "#element" do
    test "array" do
      assert_equal(GLib::VariantType.new("s"),
                   GLib::VariantType.new("as").element)
    end

    test "maybe" do
      assert_equal(GLib::VariantType.new("s"),
                   GLib::VariantType.new("ms").element)
    end

    test "unexpected" do
      assert_raise(ArgumentError.new("must be array or maybe type: <s>")) do
        GLib::VariantType.new("s").element
      end
    end
  end
end
