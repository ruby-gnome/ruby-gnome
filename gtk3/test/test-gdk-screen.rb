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

class TestGdkScreen < Test::Unit::TestCase
  sub_test_case "#get_setting" do
    test "no type" do
      screen = Gdk::Screen.default
      theme_name_value = screen.get_setting("gtk-theme-name")
      assert_equal(Gtk::Settings.default.gtk_theme_name,
                   theme_name_value.value)
    end

    test "custom type" do
      screen = Gdk::Screen.default
      double_click_time_value = screen.get_setting("gtk-double-click-time",
                                                   GLib::Type::INT)
      assert_equal(Gtk::Settings.default.gtk_double_click_time,
                   double_click_time_value.value)
    end
  end
end
