# Copyright (C) 2017 Ruby-GNOME2 Project Team
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

class TestTerminalRegex < Test::Unit::TestCase
  include VteTestUtils

  def setup
    @terminal = Vte::Terminal.new
  end

  if Vte::Version.or_later?(0, 46)
    def test_regex_for_match
      regex = Vte::Regex.new("test", GLib::RegexCompileFlags::OPTIMIZE, :for_match => true)
      assert_instance_of(Vte::Regex, regex)
    end

    def test_regex_for_match_multiple_flags
      flags = [:optimize,
               :multiline]
      regex = Vte::Regex.new("test", flags, :for_match => true)
      assert_instance_of(Vte::Regex, regex)
    end
  end
end
