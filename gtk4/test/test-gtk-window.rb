# Copyright (C) 2015-2024  Ruby-GNOME Project Team
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

class TestGtkWindow < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @window = Gtk::Window.new
  end

  def test_icon_name
    assert_nil(@window.icon_name)
    @window.icon_name = "gnome"
    assert_equal("gnome", @window.icon_name)
  end

  def test_interactive_debugging=
    Gtk::Window.interactive_debugging = false
  end
end
