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

class TestGtkActionBar < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @bar = Gtk::ActionBar.new
  end

  def test_pack_start
    widget1 = Gtk::Box.new(:horizontal)
    widget2 = Gtk::Box.new(:horizontal)
    @bar.pack_start(widget1)
    @bar.pack_start(widget2)
    assert_equal([widget1, widget2], @bar.children)
  end

  def test_pack_end
    widget1 = Gtk::Box.new(:horizontal)
    widget2 = Gtk::Box.new(:horizontal)
    @bar.pack_end(widget1)
    @bar.pack_end(widget2)
    assert_equal([widget2, widget1], @bar.children)
  end

  def test_center_widget
    widget = Gtk::Box.new(:horizontal)
    assert_nil(@bar.center_widget)
    @bar.center_widget = widget
    assert_equal(widget, @bar.center_widget)
  end
end
