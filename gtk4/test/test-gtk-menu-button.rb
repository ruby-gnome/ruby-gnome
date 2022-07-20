# Copyright (C) 2014-2022  Ruby-GNOME Project Team
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
    @menu_button = Gtk::MenuButton.new
  end

  def test_popover
    assert_nil(@menu_button.popover)
    popover = Gtk::PopoverMenu.new
    @menu_button.popover = popover
    assert_equal(popover, @menu_button.popover)
  end

  def test_menu_model
    assert_nil(@menu_button.menu_model)
  end

  def test_direction
    assert_equal(Gtk::ArrowType::DOWN, @menu_button.direction)
    @menu_button.direction = :up
    assert_equal(Gtk::ArrowType::UP, @menu_button.direction)
  end
end
