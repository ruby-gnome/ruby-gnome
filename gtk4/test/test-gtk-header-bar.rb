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

class TestGtkHeaderBar < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    only_gtk_version(3, 10, 0)
    @header_bar = Gtk::HeaderBar.new
  end

  def test_custom_title_accessors
    widget = Gtk::EventBox.new
    @header_bar.custom_title = widget
    assert_equal(widget, @header_bar.custom_title)
  end

  def test_title_accessors
    header_bar_title = "no title"
    @header_bar.title = header_bar_title
    assert_equal(header_bar_title, @header_bar.title)
  end

  def test_subtitle_accessors
    header_bar_subtitle = "sub title"
    @header_bar.subtitle = header_bar_subtitle
    assert_equal(header_bar_subtitle, @header_bar.subtitle)
  end

  def test_show_close_button_accessors
    @header_bar.show_close_button = true
    assert_equal(true, @header_bar.show_close_button?)
  end

  def test_spacing_accessors
    spacing_size = 10
    @header_bar.spacing = spacing_size
    assert_equal(spacing_size, @header_bar.spacing)
  end

  def test_pack
    start1 = Gtk::EventBox.new
    start2 = Gtk::EventBox.new
    end1 = Gtk::EventBox.new
    end2 = Gtk::EventBox.new
    @header_bar.pack_start(start1)
    @header_bar.pack_start(start2)
    @header_bar.pack_end(end1)
    @header_bar.pack_end(end2)
    assert_equal([start1, start2, end1, end2],
                 @header_bar.children)
  end

  def test_decolation_layout_accessors
    only_gtk_version(3, 12, 0)
    layout = "decolated"
    @header_bar.decoration_layout = layout
    assert_equal(layout, @header_bar.decoration_layout)
    assert_true(@header_bar.decoration_layout_set?)
  end

  def test_has_subtitle_accessors
    only_gtk_version(3, 12, 0)
    @header_bar.has_subtitle = false
    assert_false(@header_bar.has_subtitle?)
  end
end
