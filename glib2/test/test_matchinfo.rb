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
 
class TestMatchInfo < Test::Unit::TestCase
  def setup
    @regex_pattern = "[A-Z]+"
    @matching_string = "abc DEF"
    @non_matching_string = "abc def"
    @regex = GLib::Regex.new(@regex_pattern, 0, 0)
  end

  def test_string
    match_info = @regex.match(@matching_string, 0)
    assert_equal(match_info.instance_variables, [:@string])
    assert_equal(match_info.instance_variable_get("@string"), @matching_string)
    assert_equal(match_info.string, @matching_string)
  end

  def test_regex
    match_info = @regex.match(@matching_string, 0)
    mi_regex = match_info.regex
    assert_equal(mi_regex.pattern, @regex.pattern)
    assert_equal(mi_regex.compile_flags, @regex.compile_flags)
    assert_equal(mi_regex.match_flags, @regex.match_flags)
  end
  
  sub_test_case "matches" do
    test "none" do
      # find a way to generate match_info instance with no matches
      # GLib::Regex#match return nil if no matches
    end

    test "match" do
      match_info = @regex.match(@matching_string, 0)
      assert(match_info.matches)
    end
  end
end
