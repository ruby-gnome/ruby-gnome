# Copyright (C) 2025  Ruby-GNOME Project Team
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

class TestGtkPopover < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @popover = Gtk::Popover.new
  end

  def test_pointing_to
    assert_nil(@popover.pointing_to)
    @popover.pointing_to = Gdk::Rectangle.new(0, 10, 100, 20)
    assert_equal(Gdk::Rectangle.new(0, 10, 100, 20), @popover.pointing_to)
  end
end
