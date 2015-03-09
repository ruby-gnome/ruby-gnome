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

class TestGtkToolButton < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case(".new") do
    test "no argument" do
      button = Gtk::ToolButton.new
      assert_nil(button.icon_widget)
    end

    test ":icon_widget" do
      icon_widget = Gtk::EventBox.new
      button = Gtk::ToolButton.new(:icon_widget => icon_widget)
      assert_equal(icon_widget, button.icon_widget)
    end

    test ":label" do
      label = "Button"
      button = Gtk::ToolButton.new(:label => label)
      assert_equal(label, button.label)
    end

    test ":icon_widget and :label" do
      icon_widget = Gtk::EventBox.new
      label = "Button"
      button = Gtk::ToolButton.new(:icon_widget => icon_widget,
                                   :label       => label)
      assert_equal([icon_widget, label],
                   [button.icon_widget, button.label])
    end

    test ":stock_id" do
      stock_id = Gtk::Stock::OK
      button = Gtk::ToolButton.new(:stock_id => stock_id)
      assert_equal(stock_id, button.stock_id)
    end
  end
end
