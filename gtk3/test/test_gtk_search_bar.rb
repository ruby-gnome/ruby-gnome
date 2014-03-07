# Copyright (C) 2014  Ruby-GNOME2 Project Team
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

class TestGtkSearchBar < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    only_gtk_version(3, 10, 0)
    @search_bar = Gtk::SearchBar.new
  end

  def test_connect_entry
    entry = Gtk::SearchEntry.new
    assert_equal(@search_bar, @search_bar.connect_entry(entry))
  end

  def test_search_mode_enabled_accessors
    entry = Gtk::SearchEntry.new
    @search_bar.connect_entry(entry)
    @search_bar.search_mode_enabled = true
    assert_true(@search_bar.search_mode_enabled?)
  end

  def test_show_close_button_accessors
    @search_bar.show_close_button = true
    assert_true(@search_bar.show_close_button?)
  end

  def test_handle_event
    window = Gtk::Window.new
    key_press_event = Gdk::EventKey.new(:key_press)
    key_press_event.keyval = Gdk::Keyval::GDK_KEY_a
    entry = Gtk::SearchEntry.new
    @search_bar.add(entry)
    @search_bar.connect_entry(entry)
    window.add(@search_bar)
    window.show_all
    key_press_event.window = window.window
    assert_equal(Gdk::Event::STOP,
                 @search_bar.handle_event?(key_press_event))
  end
end
