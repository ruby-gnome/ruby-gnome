# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

class TestSettings < Test::Unit::TestCase
  include GioTestUtils::Fixture
  include GioTestUtils::Omissions

  sub_test_case "#get_value" do
    setup do
      @settings = Gio::Settings.new("jp.ruby-gnome2.test.value")
    end

    test "string" do
      @settings.reset("string")
      assert_equal("default-string",
                   @settings.get_value("string"))
      @settings.set_value("string", "new-string")
      assert_equal("new-string",
                   @settings.get_value("string"))
    end

    test "boolean" do
      @settings.reset("boolean")
      assert_equal(true,
                   @settings.get_value("boolean"))
      @settings.set_value("boolean", false)
      assert_equal(false,
                   @settings.get_value("boolean"))
    end
  end
end
