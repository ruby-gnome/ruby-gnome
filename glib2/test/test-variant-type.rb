# Copyright (C) 2015  Ruby-GNOME2 Project Team
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
end
