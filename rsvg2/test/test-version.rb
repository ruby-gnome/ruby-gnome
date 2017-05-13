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

class TestRsvgVersion < Test::Unit::TestCase
  include Rsvg2TestUtils

  test "STRING" do
    major = Rsvg::Version::MAJOR
    minor = Rsvg::Version::MINOR
    micro = Rsvg::Version::MICRO
    assert_equal([major, minor, micro].join("."),
                 Rsvg::Version::STRING)
  end

  sub_test_case("#or_later?") do
    test "same" do
      assert_true(Rsvg::Version.or_later?(Rsvg::Version::MAJOR,
                                          Rsvg::Version::MINOR,
                                          Rsvg::Version::MICRO))
    end

    test "later" do
      assert_true(Rsvg::Version.or_later?(Rsvg::Version::MAJOR,
                                          Rsvg::Version::MINOR - 1,
                                          Rsvg::Version::MICRO))
    end

    test "earlier" do
      assert_false(Rsvg::Version.or_later?(Rsvg::Version::MAJOR,
                                           Rsvg::Version::MINOR + 1,
                                           Rsvg::Version::MICRO))
    end
  end
end
