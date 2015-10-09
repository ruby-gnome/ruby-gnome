# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

class TestGtkClipboard < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @widget = Gtk::Invisible.new
    @clipboard = @widget.get_clipboard(Gdk::Selection::CLIPBOARD)
  end

  test "#request_contents" do
    loop = GLib::MainLoop.new
    received_text = nil
    utf8_string = Gdk::Atom.intern("UTF8_STRING")
    @clipboard.request_contents(utf8_string) do |_clipboard, _selection_data|
      compound_text = Gdk::Atom.intern("COMPOUND_TEXT")
      @clipboard.request_contents(compound_text) do |_clipboard, selection_data|
        received_text = selection_data.text
        GLib::Idle.add do
          loop.quit
          GLib::Source::REMOVE
        end
      end
    end
    @clipboard.text = "hello"
    loop.run

    assert_equal("hello", received_text)
  end

  test "#request_text" do
    loop = GLib::MainLoop.new
    received_text = nil
    @clipboard.request_text do |_clipboard, text|
      received_text = text
      GLib::Idle.add do
        loop.quit
        GLib::Source::REMOVE
      end
    end
    @clipboard.text = "hello"
    loop.run

    assert_equal("hello", received_text)
  end
end
