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

class TestGtkHeaderBar < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @header_bar = Gtk::HeaderBar.new
  end

  def test_title_widget_accessors
    widget = Gtk::Label.new("title")
    @header_bar.title_widget = widget
    assert_equal(widget, @header_bar.title_widget)
  end

  def test_show_title_buttons_accessors
    @header_bar.show_title_buttons = true
    assert do
      @header_bar.show_title_buttons?
    end
  end

  def test_pack
    start1 = Gtk::Label.new("start1")
    start2 = Gtk::Label.new("start2")
    end1 = Gtk::Label.new("end1")
    end2 = Gtk::Label.new("end2")
    @header_bar.pack_start(start1)
    @header_bar.pack_start(start2)
    @header_bar.pack_end(end1)
    @header_bar.pack_end(end2)
    assert_equal([
                   [start1, start2],
                   [end2, end1],
                 ],
                 [
                   start1.parent.children[1..-1],
                   end1.parent.children[0..-2],
                 ])
  end

  def test_decolation_layout_accessors
    layout = "decolated"
    @header_bar.decoration_layout = layout
    assert_equal(layout, @header_bar.decoration_layout)
  end
end
