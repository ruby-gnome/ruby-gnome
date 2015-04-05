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

class TestGtkActionBar < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    only_gtk_version(3, 12, 0)
    @action_bar = Gtk::ActionBar.new
  end

  def test_center_widget
    widget = Gtk::EventBox.new
    @action_bar.center_widget = widget
    assert_equal(widget, @action_bar.center_widget)
  end

  def test_pack
    start1 = Gtk::EventBox.new
    start2 = Gtk::EventBox.new
    end1 = Gtk::EventBox.new
    end2 = Gtk::EventBox.new
    @action_bar.pack_start(start1)
    @action_bar.pack_start(start2)
    @action_bar.pack_end(end1)
    @action_bar.pack_end(end2)
    assert_equal([start1, start2, end2, end1],
                 @action_bar.children)
  end
end
