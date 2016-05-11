# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

class TestPangoContext < Test::Unit::TestCase
  include GtkTestUtils

  def test_set_shape_renderer
    text = "this is a test"
    label = Gtk::Label.new(text)
    layout = label.layout

    last_block_arguments = nil
    layout.context.set_shape_renderer do |cr, attr, do_path|
      last_block_arguments = [cr.class, attr.class, do_path]
    end

    metrics = layout.context.get_metrics(layout.font_description)
    ascent = metrics.ascent
    logical_rect = Pango::Rectangle.new(0, -ascent, ascent, ascent)
    ink_rect = logical_rect.dup
    attrs = Pango::AttrList.new
    attr = Pango::AttrShape.new(ink_rect, logical_rect, "a")
    attr.start_index = text[0, text.index("a")].bytesize
    attr.end_index = attr.start_index + "a".bytesize
    attrs.insert(attr)
    label.attributes = attrs

    window = Gtk::Window.new
    window.signal_connect("destroy") do
      Gtk.main_quit
    end
    window.add(label)
    GLib::Timeout.add(1000) do
      window.destroy
      GLib::Source::REMOVE
    end
    window.show_all
    Gtk.main

    assert_equal([Cairo::Context, Pango::AttrShape, false],
                 last_block_arguments)
  end
end
