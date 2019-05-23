# Copyright (C) 2019  Ruby-GNOME2 Project Team
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

class TestWnckVersion < Test::Unit::TestCase
  test "STRING" do
    major = Wnck::Version::MAJOR
    minor = Wnck::Version::MINOR
    micro = Wnck::Version::MICRO
    assert_equal([major, minor, micro].join("."),
                 Wnck::Version::STRING)
  end

  sub_test_case("#or_later?") do
    test "same" do
      assert_true(Wnck::Version.or_later?(Wnck::Version::MAJOR,
                                          Wnck::Version::MINOR,
                                          Wnck::Version::MICRO))
    end

    test "later" do
      assert_true(Wnck::Version.or_later?(Wnck::Version::MAJOR,
                                          Wnck::Version::MINOR - 1,
                                          Wnck::Version::MICRO))
    end

    test "earlier" do
      assert_false(Wnck::Version.or_later?(Wnck::Version::MAJOR,
                                           Wnck::Version::MINOR + 1,
                                           Wnck::Version::MICRO))
    end
  end
end
