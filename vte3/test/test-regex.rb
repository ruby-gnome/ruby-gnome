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

class TestRegex < Test::Unit::TestCase
  include VteTestUtils

  def setup
    only_vte_version(0, 46)
  end

  def test_for_match
    Vte::Regex.new("test", 0, :for_match => true)
    # TODO: Add assertion
  end

  def test_for_search
    Vte::Regex.new("test", 0, :for_search => true)
    # TODO: Add assertion
  end

  sub_test_case("flags") do
    def test_integer
      Vte::Regex.new("test",
                     GLib::RegexCompileFlags::OPTIMIZE.to_i,
                     :for_match => true)
      # TODO: Add assertion
    end

    def test_object
      Vte::Regex.new("test",
                     GLib::RegexCompileFlags::OPTIMIZE,
                     :for_match => true)
      # TODO: Add assertion
    end

    def test_array
      flags = [:optimize, :multiline]
      Vte::Regex.new("test", flags, :for_match => true)
      # TODO: Add assertion
    end
  end
end
