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


class TestSimpleAction < Test::Unit::TestCase
  sub_test_case "#initialize" do
    sub_test_case "parameter type" do
      test "omit" do
        action = Gio::SimpleAction.new("open")
        assert_nil(action.parameter_type)
      end

      test "nil" do
        action = Gio::SimpleAction.new("open", nil)
        assert_nil(action.parameter_type)
      end

      test "String" do
        action = Gio::SimpleAction.new("open", "s")
        assert_equal(GLib::VariantType::STRING,
                     action.parameter_type)
      end

      test "GLib::VariantType" do
        action = Gio::SimpleAction.new("open", GLib::VariantType::STRING)
        assert_equal(GLib::VariantType::STRING,
                     action.parameter_type)
      end
    end
  end
end
