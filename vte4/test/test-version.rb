# Copyright (C) 2015-2023  Ruby-GNOME Project Team
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

class TestVteVersion < Test::Unit::TestCase
  include VteTestUtils

  test "STRING" do
    major = Vte::Version::MAJOR
    minor = Vte::Version::MINOR
    micro = Vte::Version::MICRO
    assert_equal([major, minor, micro].join("."),
                 Vte::Version::STRING)
  end

  sub_test_case("#or_later?") do
    test "same" do
      assert do
        Vte::Version.or_later?(Vte::Version::MAJOR,
                               Vte::Version::MINOR,
                               Vte::Version::MICRO)
      end
    end

    test "later" do
      assert do
        Vte::Version.or_later?(Vte::Version::MAJOR,
                               Vte::Version::MINOR - 1,
                               Vte::Version::MICRO)
      end
    end

    test "earlier" do
      assert do
        not Vte::Version.or_later?(Vte::Version::MAJOR,
                                   Vte::Version::MINOR + 1,
                                   Vte::Version::MICRO)
      end
    end
  end
end
