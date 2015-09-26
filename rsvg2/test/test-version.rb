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

class TestRSVGVersion < Test::Unit::TestCase
  include RSVG2TestUtils

  test "STRING" do
    major = RSVG::Version::MAJOR
    minor = RSVG::Version::MINOR
    micro = RSVG::Version::MICRO
    assert_equal([major, minor, micro].join("."),
                 RSVG::Version::STRING)
  end

  sub_test_case("#or_later?") do
    test "same" do
      assert_true(RSVG::Version.or_later?(RSVG::Version::MAJOR,
                                          RSVG::Version::MINOR,
                                          RSVG::Version::MICRO))
    end

    test "later" do
      assert_true(RSVG::Version.or_later?(RSVG::Version::MAJOR,
                                          RSVG::Version::MINOR - 1,
                                          RSVG::Version::MICRO))
    end

    test "earlier" do
      assert_false(RSVG::Version.or_later?(RSVG::Version::MAJOR,
                                           RSVG::Version::MINOR + 1,
                                           RSVG::Version::MICRO))
    end
  end
end
