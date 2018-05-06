# Copyright (C) 2018  Ruby-GNOME2 Project Team
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

class TestMenuItem < Test::Unit::TestCase
  include GioTestUtils::Omissions

  def setup
    @menu_item = Gio::MenuItem.new
  end

  sub_test_case "attribute value" do
    test "action" do
      only_gio_version(2, 32, 0)
      assert_nil(@menu_item.action)
      @menu_item.action = "win."
      assert_equal("win.", @menu_item.action)
    end

    test "action-namespace" do
      only_gio_version(2, 36, 0)
      assert_nil(@menu_item.action_namespace)
      @menu_item.action_namespace = "doc"
      assert_equal("doc", @menu_item.action_namespace)
    end
  end
end
