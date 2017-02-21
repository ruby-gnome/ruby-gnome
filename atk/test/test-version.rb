# Copyright (C) 2017  Ruby-GNOME2 Project Team
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

class TestAtkVersion < Test::Unit::TestCase
  include AtkTestUtils

  test "STRING" do
    major = Atk::Version::MAJOR
    minor = Atk::Version::MINOR
    micro = Atk::Version::MICRO
    assert_equal([major, minor, micro].join("."),
                 Atk::Version::STRING)
  end

  sub_test_case("#or_later?") do
    test "same" do
      assert_true(Atk::Version.or_later?(Atk::Version::MAJOR,
                                             Atk::Version::MINOR,
                                             Atk::Version::MICRO))
    end

    test "later" do
      assert_true(Atk::Version.or_later?(Atk::Version::MAJOR,
                                         Atk::Version::MINOR - 1,
                                         Atk::Version::MICRO))
    end

    test "earlier" do
      assert_false(Atk::Version.or_later?(Atk::Version::MAJOR,
                                          Atk::Version::MINOR + 1,
                                          Atk::Version::MICRO))
    end
  end
end
