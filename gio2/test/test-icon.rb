# Copyright (C) 2019  Ruby-GNOME Project Team
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

class TestIcon < Test::Unit::TestCase
  include GioTestUtils::Fixture
  include GioTestUtils::Omissions

  sub_test_case(".find") do
    def test_found
      assert_equal(Gio::ThemedIcon.new("open"),
                   Gio::Icon.find("open"))
    end

    def test_not_found
      Gio::Icon.find("nonexistent")
    end
  end

  sub_test_case(".hash") do
    def test_use_as_key
      hash = {Gio::Icon => true}
      assert do
        hash[Gio::Icon]
      end
    end
  end

  sub_test_case("#hash") do
    def test_use_as_key
      hash = {Gio::Icon.find("open") => true}
      assert do
        hash[Gio::Icon.find("open")]
      end
    end
  end
end
