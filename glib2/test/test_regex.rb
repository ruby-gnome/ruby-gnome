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
 
class TestRegex < Test::Unit::TestCase

  def test_regex_enum_compile_flags
    assert_const_defined(GLib, :RegexCompileFlags)
    assert_kind_of(GLib::RegexCompileFlags, GLib::RegexCompileFlags::CASELESS)
  end

  def test_regex_enum_match_flags
    assert_const_defined(GLib, :RegexMatchFlags)
    assert_kind_of(GLib::RegexMatchFlags, GLib::RegexMatchFlags::PARTIAL_HARD)
  end

  def test_regex_new
    a_regex = GLib::Regex.new("to??", 0, 0)
    assert_instance_of(GLib::Regex, a_regex)
  end

  def test_regex_pattern
    a_regex = GLib::Regex.new("to??", 0, 0)
    assert_equal(a_regex.pattern, "to??")
  end

  def test_regex_compile_flags
    flags = GLib::RegexCompileFlags::CASELESS
    a_regex = GLib::Regex.new("to??", flags, 0)
    assert_equal(a_regex.compile_flags, flags) 
  end

  def test_regex_match_flags
    flags = GLib::RegexMatchFlags::PARTIAL_HARD
    a_regex = GLib::Regex.new("to??", 0, flags)
    assert_equal(a_regex.match_flags, flags) 
  end

  sub_test_case "max_backref" do
    test "none" do
      a_regex = GLib::Regex.new("to?o", 0, 0)
      assert_equal(a_regex.max_backref, 0)
    end
    test "one" do
      a_regex = GLib::Regex.new("(to(?)o)\\g1", 0, 0)
      assert_equal(a_regex.max_backref, 1)
    end
  end

  sub_test_case "match_simple" do
    test "true" do
      matched = GLib::Regex.match_simple("to", "tatota", 0, 0)
      assert(matched)
    end
    test "false" do
      matched = GLib::Regex.match_simple("ti", "tatota", 0, 0)
      assert_equal(matched, false)
    end
  end

  sub_test_case "capture_count" do
    test "none" do
      a_regex = GLib::Regex.new("to?o", 0, 0)
      assert_equal(a_regex.capture_count, 0)
    end
    test "one" do
      a_regex = GLib::Regex.new("(to(\?)o)", 0, 0)
      assert_equal(a_regex.capture_count, 1)
    end
    test "three" do
      a_regex = GLib::Regex.new("((red|white) (king|queen))", 0, 0)
      assert_equal(a_regex.capture_count, 3)
    end
  end

  sub_test_case "has_cr_or_lf" do
    test "none" do
      a_regex = GLib::Regex.new("to?o", 0, 0)
      assert_equal(a_regex.has_cr_or_lf, false)
    end
    test "both" do
      a_regex = GLib::Regex.new("to\no", 0, 0)
      assert(a_regex.has_cr_or_lf)
    end
    test "cr" do
      a_regex = GLib::Regex.new("to\rtiti", 0, 0)
      assert(a_regex.has_cr_or_lf)
    end
  end
  
  sub_test_case "max_lookbehind" do
    test "none" do
      a_regex = GLib::Regex.new("to?o", 0, 0)
      assert_equal(a_regex.max_lookbehind, 0)
    end
    test "three" do
      a_regex = GLib::Regex.new("(?<!foo)bar", 0, 0)
      assert_equal(a_regex.max_lookbehind, 3)
    end
  end

  sub_test_case "string_number" do
    test "none" do
      a_regex = GLib::Regex.new("too", 0, 0)
      assert_equal(a_regex.string_number("a_name"), -1)
    end
    test "one" do
      a_regex = GLib::Regex.new("(?<a_name>foo)bar", 0, 0)
      assert_equal(a_regex.string_number("a_name"), 1)
    end
    test "two" do
      a_regex = GLib::Regex.new("(?<another_name>foo)(?<a_name>bar)", 0, 0)
      assert_equal(a_regex.string_number("a_name"), 2)
    end
  end

  def test_regex_escape_string
    assert_equal(GLib::Regex.escape_string("a.b*c"), "a\\.b\\*c")
  end

  def test_regex_escape_null
    assert_equal(String.new("ab\0c"), "ab\x00c")
  end

  def test_regex_split
    a_regex = GLib::Regex.new("\s", 0, 0)
    string_to_split = "a bc"
    splited_strings = a_regex.split(string_to_split, 0)
    assert_equal(splited_strings, ["a", "bc"])
  end

  def test_regex_split_full
    a_regex = GLib::Regex.new("\s", 0, 0)
    string_to_split = "a bc"
    splited_strings = a_regex.split_full(string_to_split, 0, 0, 0)
    assert_equal(splited_strings, ["a", "bc"])
  end

  def test_regex_replace
    a_regex = GLib::Regex.new("\s", 0, 0)
    string_to_modify = "a bc"
    replacement ="_"
    modified_string = a_regex.replace(string_to_modify, 0, replacement, 0)
    assert_equal(modified_string, "a_bc")
  end

  def test_regex_replace_literal
    a_regex = GLib::Regex.new("\s", 0, 0)
    string_to_modify = "a bc"
    replacement ="_"
    modified_string = a_regex.replace_literal(string_to_modify, 0, replacement, 0)
    assert_equal(modified_string, "a_bc")
  end
  
  def test_regex_replace_eval
    a_regex = GLib::Regex.new("1|2|3|4", 0, 0)
    replacement_strings = { "1" => "one",
                            "2" => "two",
                            "3" => "three",
                            "4" => "four"
                            }
    string_to_modify = " 4 3 2 1"

    modified_string = a_regex.replace_eval(string_to_modify, 0, 0) do |info, result|
      assert_instance_of(GLib::MatchInfo, info)
      "to"
    end
    assert_equal(modified_string, "to 3 2 1")

    modified_string = a_regex.replace_eval(string_to_modify, 0, 0) do |info, result|
      assert_instance_of(GLib::MatchInfo, info)
      ["to"]
    end
    assert_equal(modified_string, "to 3 2 1")

    modified_string = a_regex.replace_eval(string_to_modify, 0, 0) do |info, result|
      assert_instance_of(GLib::MatchInfo, info)
      ["to", false]
    end
    assert_equal(modified_string, "to   ")

    modified_string = a_regex.replace_eval(string_to_modify, 0, 0) do |info, result|
      assert_instance_of(GLib::MatchInfo, info)
      ["to", true]
    end
    assert_equal(modified_string, "to 3 2 1")

    modified_string = a_regex.replace_eval(string_to_modify, 0, 0) do |info, result|
      assert_instance_of(GLib::MatchInfo, info)
      [result + "to", true]
    end
    assert_equal(modified_string, " to 3 2 1")

    modified_string = a_regex.replace_eval(string_to_modify, 0, 0) do |info, result|
      assert_instance_of(GLib::MatchInfo, info)
      [result + "to", false]
    end
    assert_equal(modified_string, " to to to to")
  end

  sub_test_case "match" do
    a_regex = GLib::Regex.new("[A-Z]+", 0, 0)
    
    test "no match" do
      assert_equal(nil, a_regex.match("abc def", 0))
    end
    
    test "match" do
      assert_instance_of(GLib::MatchInfo, a_regex.match("abc DEF", 0))
    end
  
    test "match longer" do
      a_regex = GLib::Regex.new("<.*>", 0, 0)
      # TODO test matching "<a> <b> <c>" against the pattern "<.*>;" you would obtain one matche: "<a> <b> <c>"
    end
end

  sub_test_case "match_full" do
    a_regex = GLib::Regex.new("[A-Z]+", 0, 0)
    
    test "no match" do
      assert_equal(nil, a_regex.match_full("abc def", 0, 0))
    end
    
    test "match" do
      assert_instance_of(GLib::MatchInfo, a_regex.match_full("abc DEF", 0, 0))
    end
    
    test "match longer" do
      a_regex = GLib::Regex.new("<.*>", 0, 0)
      # TODO test matching "<a> <b> <c>" against the pattern "<.*>;" you would obtain one matche: "<a> <b> <c>"
    end
  end

  sub_test_case "match_all" do
    a_regex = GLib::Regex.new("[A-Z]+", 0, 0)
    
    test "no match" do
      assert_equal(nil, a_regex.match_all("abc def", 0))
    end
    
    test "match" do
      assert_instance_of(GLib::MatchInfo, a_regex.match_all("abc DEF", 0))
    end
    
    test "match all" do
      a_regex = GLib::Regex.new("<.*>", 0, 0)
      # TODO test matching "<a> <b> <c>" against the pattern "<.*>;" you would obtain three matches: "<a> <b> <c>", "<a> <b>" and "<a>".
    end
  end
  
  sub_test_case "match_all_full" do
    a_regex = GLib::Regex.new("[A-Z]+", 0, 0)
    
    test "no match" do
      assert_equal(nil, a_regex.match_all_full("abc def", 0, 0))
    end
    
    test "match" do
      assert_instance_of(GLib::MatchInfo, a_regex.match_all_full("abc DEF", 0, 0))
    end

    test "match all" do
      a_regex = GLib::Regex.new("<.*>", 0, 0)
      # TODO test matching "<a> <b> <c>" against the pattern "<.*>;" you would obtain three matches: "<a> <b> <c>", "<a> <b>" and "<a>".
    end
  end

  def test_regex_split_simple
    pattern = "\s*"
    string_to_split = "a bc"
    splited_strings = GLib::Regex.split_simple(pattern, string_to_split, 0, 0)
    assert_equal(splited_strings, ["a", "b", "c"])
  end

  sub_test_case "check_replacement" do
    test "no references" do
      assert_equal([true, false], GLib::Regex.check_replacement("foo\n"))
    end
    test "with references" do
      assert_equal([true, true], GLib::Regex.check_replacement("\\0\\1"))
    end
  end
end
