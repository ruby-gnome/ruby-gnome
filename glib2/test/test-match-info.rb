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

class TestMatchInfo < Test::Unit::TestCase
  include GLibTestUtils

  def test_string
    regex = GLib::Regex.new("[A-Z]+")
    match_info = regex.match("abc def")
    assert_equal("abc def", match_info.string)
  end

  def test_regex
    regex = GLib::Regex.new("[A-Z]+")
    match_info = regex.match("abc def")
    assert_equal("[A-Z]+", match_info.regex.pattern)
  end

  def test_partial_match
    flags = GLib::RegexMatchFlags::PARTIAL_SOFT
    regex = GLib::Regex.new("jan")
    match_info = regex.match_all("ja", :match_options => flags)
    assert do
      !match_info.matches?
    end
    assert do
      match_info.partial_match?
    end
  end

  sub_test_case "fetch" do
    test "Integer" do
      regex = GLib::Regex.new("[A-Z]+")
      match_info = regex.match_all("abc DEF ghi JKL mnop")
      assert_equal("DEF", match_info.fetch(0))
      assert_equal("DE", match_info.fetch(1))
    end

    test "String" do
      regex = GLib::Regex.new("(?<a_name>fo+)")
      match_info = regex.match("tatafoo")
      assert_equal("foo", match_info.fetch("a_name"))
    end

    test "Symbol" do
      regex = GLib::Regex.new("(?<a_name>fo+)")
      match_info = regex.match("tatafoo")
      assert_equal("foo", match_info.fetch(:a_name))
    end

    test "[]" do
      regex = GLib::Regex.new("(?<a_name>fo+)")
      match_info = regex.match("tatafoo")
      assert_equal("foo", match_info[:a_name])
    end
  end

  sub_test_case "fetch_pos" do
    test "Integer" do
      regex = GLib::Regex.new("[A-Z]+")
      match_info = regex.match_all("abc DEF ghi JKL mnop")
      assert_equal([4, 7], match_info.fetch_pos(0))
      assert_equal([4, 6], match_info.fetch_pos(1))
    end

    test "String" do
      regex = GLib::Regex.new("(?<a_name>fo+)")
      match_info = regex.match("tatafoo")
      assert_equal([4, 7], match_info.fetch_pos("a_name"))
    end

    test "Symbol" do
      regex = GLib::Regex.new("(?<a_name>fo+)")
      match_info = regex.match("tatafoo")
      assert_equal([4, 7], match_info.fetch_pos(:a_name))
    end
  end

  def test_fetch_all
    regex = GLib::Regex.new("[A-Z]+")
    str = "abc DEF ghi JKL mnop"
    match_info = regex.match_all(str)
    assert_equal(["DEF", "DE", "D"], match_info.fetch_all)
  end

  def test_next
    regex = GLib::Regex.new("[A-Z]+")
    str = "abc DEF ghi JKL mnop"
    match_info = regex.match(str)
    assert_equal("DEF", match_info[0])
    assert {match_info.next}
    assert_equal("JKL", match_info[0])
    assert {!match_info.next}
  end

  def test_expand_references
    regex = GLib::Regex.new("a(?P<G>.)c")
    match_info = regex.match("xabcy")
    expanded_string = match_info.expand_references("X\\g<G>X")
    assert_equal("XbX", expanded_string)
  end
end
