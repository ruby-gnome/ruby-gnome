# Copyright (C) 2014 Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

class TestGtkMenuButton < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    only_gtk_version(3, 6, 0)
    @menu_button = Gtk::MenuButton.new
  end

  def test_popup
    assert_nil(@menu_button.popup)
    popup = Gtk::Menu.new
    @menu_button.popup = popup
    assert_equal(popup, @menu_button.popup)
  end

  def test_menu_model
    assert_nil(@menu_button.menu_model)
  end

  def test_direction
    assert_equal(Gtk::Arrow::Type::DOWN, @menu_button.direction)
    @menu_button.direction = :up
    assert_equal(Gtk::Arrow::Type::UP, @menu_button.direction)
  end

  def test_align_widget
    assert_nil(@menu_button.align_widget)
    align_widget = Gtk::Box.new(:horizontal)
    align_widget.add(@menu_button)
    @menu_button.align_widget = align_widget
    assert_equal(align_widget, @menu_button.align_widget)
  end
end
