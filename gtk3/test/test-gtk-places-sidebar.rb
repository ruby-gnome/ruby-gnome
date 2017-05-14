# Copyright (C) 2014-2017 Ruby-GNOME2 Project Team
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

class TestGtkPlacesSidebar < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    only_gtk_version(3, 10, 0)
    @places_sidebar = Gtk::PlacesSidebar.new
  end

  def test_open_flags_accessors
    open_flag_new_tab = Gtk::PlacesOpenFlags::NEW_TAB
    @places_sidebar.open_flags = open_flag_new_tab
    assert_equal(open_flag_new_tab, @places_sidebar.open_flags)
  end

  def test_location_accessors
    assert_nil(@places_sidebar.location)

    home = Gio::File.open(path: ENV["HOME"])
    @places_sidebar.location = home
    assert_equal(home.path, @places_sidebar.location.path)
  end

  def test_show_desktop_accessors
    @places_sidebar.show_desktop = false
    assert_false(@places_sidebar.show_desktop?)
  end

  def test_shortcuts
    file_location1 = Gio::File.open(path: "file1")
    file_location2 = Gio::File.open(path: "file2")
    @places_sidebar.add_shortcut(file_location1)
    @places_sidebar.add_shortcut(file_location2)
    assert_equal([file_location1, file_location2],
                 @places_sidebar.shortcuts)
  end

  def test_local_only_accesssors
    @places_sidebar.local_only = true
    assert_true(@places_sidebar.local_only?)
  end

  def test_show_enter_location_accesssors
    only_gtk_version(3, 14, 0)
    @places_sidebar.show_enter_location = true
    assert_true(@places_sidebar.show_enter_location?)
  end

  def test_show_connect_to_server_accesssors
    @places_sidebar.show_connect_to_server = true
    assert_true(@places_sidebar.show_connect_to_server?)
  end
end
