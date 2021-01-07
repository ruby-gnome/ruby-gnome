# Copyright (C) 2016-2021  Ruby-GNOME Project Team
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

class TestTimeZone < Test::Unit::TestCase
  include GLibTestUtils

  sub_test_case "new" do
    test "local" do
      local_1 = GLib::TimeZone.local
      local_2 = GLib::TimeZone.new
      assert_equal(local_1.abbreviation(0), local_2.abbreviation(0))
    end

    test "UTC" do
      utc_1 = GLib::TimeZone.utc
      utc_2 = GLib::TimeZone.new("UTC")
      assert_equal(utc_1.offset(0), utc_2.offset(0))
    end
  end
end
