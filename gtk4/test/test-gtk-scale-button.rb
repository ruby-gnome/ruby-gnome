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

class TestGtkScaleButton < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case(".new") do
    test "no argument" do
      scale_button = Gtk::ScaleButton.new
      assert_equal(Gtk::IconSize::BUTTON, scale_button.size)
      assert_equal(0.0, scale_button.adjustment.lower)
      assert_equal(100.0, scale_button.adjustment.upper)
      assert_equal(2.0, scale_button.adjustment.step_increment)
      assert_equal(nil, scale_button.icons)
    end

    test ":icon_size" do
      scale_button = Gtk::ScaleButton.new(:icon_size => :menu)
      assert_equal(Gtk::IconSize::MENU, scale_button.size)
    end

    test ":min" do
      scale_button = Gtk::ScaleButton.new(:min => 4.0)
      assert_equal(4.0, scale_button.adjustment.lower)
    end

    test ":max" do
      scale_button = Gtk::ScaleButton.new(:max => 90.0)
      assert_equal(90.0, scale_button.adjustment.upper)
    end

    test ":step" do
      scale_button = Gtk::ScaleButton.new(:step => 5.0)
      assert_equal(5.0, scale_button.adjustment.step_increment)
    end

    test ":icons" do
      icons_set = %w(gtk-goto-bottom gtk-goto-top gtk-execute)
      scale_button = Gtk::ScaleButton.new(:icons => icons_set)
      assert_equal(icons_set, scale_button.icons)
    end
  end
end
