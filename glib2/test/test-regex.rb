# Copyright (C) 2015-2016  Ruby-GNOME2 Project Team
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

class TestRegex < Test::Unit::TestCase
  def test_enum_match_flags
    assert_const_defined(GLib, :RegexMatchFlags)
    assert_kind_of(GLib::RegexMatchFlags, GLib::RegexMatchFlags::PARTIAL_HARD)
  end

  def test_enum_compile_flags
    assert_const_defined(GLib, :RegexCompileFlags)
    assert_kind_of(GLib::RegexCompileFlags, GLib::RegexCompileFlags::CASELESS)
  end

  def test_pattern
    regex = GLib::Regex.new("to??")
    assert_equal("to??", regex.pattern)
  end

  def test_compile_flags
    flags = GLib::RegexCompileFlags::CASELESS
    regex = GLib::Regex.new("to??", :compile_options => flags)
    assert_equal(flags, regex.compile_flags)
  end

  def test_match_flags
    flags = GLib::RegexMatchFlags::PARTIAL_HARD
    regex = GLib::Regex.new("to??", :match_options => flags)
    assert_equal(flags, regex.match_flags)
  end

  sub_test_case "split" do
    test "no options" do
      regex = GLib::Regex.new("\s")
      string_to_split = "a bc"
      splited_strings = regex.split(string_to_split)
      assert_equal(["a", "bc"], splited_strings)
    end

    test "start_position" do
      regex = GLib::Regex.new("\s")
      string_to_split = "a bc"
      splited_strings = regex.split(string_to_split, :start_position => 2)
      assert_equal(["bc"], splited_strings)
    end

    test "max_tokens" do
      regex = GLib::Regex.new("\s")
      string_to_split = "a bc de fg"
      splited_strings = regex.split(string_to_split, :max_tokens => 2)
      assert_equal(["a", "bc de fg"], splited_strings)
    end

    test "match_options" do
      regex = GLib::Regex.new("a?b?")
      string_to_split = "toto ab"
      splited_strings = regex.split(string_to_split)
      assert_equal(["t", "o", "t", "o", " "],
                   splited_strings)
      splited_strings = regex.split(string_to_split,
                                    :match_options => :notempty)
      assert_equal(["toto ", ""], splited_strings)
    end
  end

  sub_test_case "match" do
    setup do
      @regex = GLib::Regex.new("[A-Z]+")
    end

    test "no match no options" do
      match_info = @regex.match("abc def")
      assert do
        not match_info.matches?
      end
    end

    test "matched no options" do
      match_info = @regex.match("abc DEF")
      assert do
        match_info.matches?
      end
    end

    test "no match and start position option" do
      match_info = @regex.match("abc def", :start_position => 4)
      assert do
        not match_info.matches?
      end
    end

    test "matched and start position option" do
      match_info = @regex.match("abc DEF", :start_position => 4)
      assert do
        match_info.matches?
      end
    end
  end

  sub_test_case "max_backref" do
    test "none" do
      regex = GLib::Regex.new("to?o")
      assert_equal(0, regex.max_backref)
    end

    test "one" do
      regex = GLib::Regex.new("(to(?)o)\\g1")
      assert_equal(1, regex.max_backref)
    end
  end

  sub_test_case "capture_count" do
    test "none" do
      regex = GLib::Regex.new("to?o")
      assert_equal(0, regex.capture_count)
    end

    test "one" do
      regex = GLib::Regex.new("(to(\?)o)")
      assert_equal(1, regex.capture_count)
    end

    test "three" do
      regex = GLib::Regex.new("((red|white) (king|queen))")
      assert_equal(3, regex.capture_count)
    end
  end

  sub_test_case "has_cr_or_lf" do
    test "none" do
      regex = GLib::Regex.new("to?o")
      assert do
        not regex.has_cr_or_lf?
      end
    end

    test "both" do
      regex = GLib::Regex.new("to\no")
      assert do
        regex.has_cr_or_lf?
      end
    end

    test "cr" do
      regex = GLib::Regex.new("to\rtiti")
      assert do
        regex.has_cr_or_lf?
      end
    end
  end

  sub_test_case "max_lookbehind" do
    test "none" do
      regex = GLib::Regex.new("to?o")
      assert_equal(0, regex.max_lookbehind)
    end

    test "three" do
      regex = GLib::Regex.new("(?<!foo)bar")
      assert_equal(3, regex.max_lookbehind)
    end
  end

  sub_test_case "string_number" do
    test "none" do
      regex = GLib::Regex.new("too")
      assert_equal(-1, regex.string_number("a_name"))
    end

    test "one" do
      regex = GLib::Regex.new("(?<a_name>foo)bar")
      assert_equal(1, regex.string_number("a_name"))
    end

    test "two" do
      regex = GLib::Regex.new("(?<another_name>foo)(?<a_name>bar)")
      assert_equal(2, regex.string_number("a_name"))
    end
  end

  def test_regex_escape_string
    assert_equal("a\\.b\\*c", GLib::Regex.escape_string("a.b*c"))
  end

  sub_test_case "match_simple" do
    test "true" do
      matched = GLib::Regex.match_simple("to", "tatota")
      assert(matched)
    end
    test "false" do
      matched = GLib::Regex.match_simple("ti", "tatota")
      assert_equal(matched, false)
    end
  end
end
