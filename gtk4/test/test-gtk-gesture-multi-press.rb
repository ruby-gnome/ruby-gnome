# Copyright (C) 2014-2015 Ruby-GNOME2 Project Team
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

class TestGtkGestureMultiPress < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    only_gtk_version(3, 14, 0)
    widget = Gtk::Invisible.new
    @press = Gtk::GestureMultiPress.new(widget)
  end

  def test_area
    rectangle = Gdk::Rectangle.new(0, 10, 20, 30)
    @press.area = rectangle
    assert_equal(rectangle, @press.area)
  end
end
