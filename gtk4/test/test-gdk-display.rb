# Copyright (C) 2015-2022  Ruby-GNOME Project Team
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

class TestGdkDisplay < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @display = Gdk::Display.default
  end

  test("#device_is_grabbed?") do
    device = @display.default_seat.pointer
    assert do
      not @display.device_is_grabbed?(device)
    end
  end

  sub_test_case("#get_setting") do
    test("no type") do
      theme_name_value = GLib::Value.new(GLib::Type::STRING, "")
      success = @display.get_setting("gtk-theme-name", theme_name_value)
      if (not success) and x11?
        omit("XSETTINGS is required.")
      end
      assert_equal(Gtk::Settings.default.gtk_theme_name,
                   theme_name_value.value)
    end

    test("custom type") do
      double_click_time_value = GLib::Value.new(GLib::Type::INT, 0)
      success = @display.get_setting("gtk-double-click-time", double_click_time_value)
      if (not success) and x11?
        omit("XSETTINGS is required.")
      end
      assert_equal(Gtk::Settings.default.gtk_double_click_time,
                   double_click_time_value.value)
    end
  end

  sub_test_case("#add_style_provider") do
    def setup
      super
      @style_provider = Gtk::CssProvider.new
    end

    test("provider, Integer") do
      @display.add_style_provider(@style_provider,
                                 Gtk::StyleProvider::PRIORITY_APPLICATION)
    end

    test("provider, Symbol") do
      @display.add_style_provider(@style_provider, :application)
    end

    test("provider") do
      @display.add_style_provider(@style_provider)
    end
  end

  test("#remove_style_provider") do
    style_provider = Gtk::CssProvider.new
    @display.add_style_provider(style_provider,
                               Gtk::StyleProvider::PRIORITY_APPLICATION)
    @display.remove_style_provider(style_provider)
  end
end
