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
      assert_equal(splited_strings, ["a", "bc"])
    end

    test "start_position" do
      regex = GLib::Regex.new("\s")
      string_to_split = "a bc"
      splited_strings = regex.split(string_to_split, :start_position => 2)
      assert_equal(splited_strings, ["bc"])
    end

    test "max_tokens" do
      regex = GLib::Regex.new("\s")
      string_to_split = "a bc de fg"
      splited_strings = regex.split(string_to_split, :max_tokens => 2)
      assert_equal(splited_strings, ["a","bc de fg"])
    end

    test "match_options" do
      regex = GLib::Regex.new("a?b?")
      string_to_split = "toto ab"
      splited_strings = regex.split(string_to_split)
      assert_equal(splited_strings, ["t","o" ,"t", "o"," "])
      splited_strings = regex.split(string_to_split,
                                    :match_options => :notempty)
      assert_equal(splited_strings, ["toto ", ""])
    end
  end

  sub_test_case "match" do
    setup do
      @regex = GLib::Regex.new("[A-Z]+")
    end

    # TODO: Move to test-match-info.rb
    test "MatchInfo#string" do
      match_info = @regex.match("abc def")
      assert_equal(match_info.string, "abc def")
    end

    # TODO: Move to test-match-info.rb
    test "MatchInfo#regex" do
      match_info = @regex.match("abc def")
      assert_equal(match_info.regex.pattern, @regex.pattern)
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
end
