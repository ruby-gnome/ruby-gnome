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

  def teardown
    @clipboard.clear
  end

  test "#request_contents" do
    loop = GLib::MainLoop.new
    received_text = nil
    utf8_string = Gdk::Atom.intern("UTF8_STRING")
    @clipboard.request_contents(utf8_string) do |_clipboard, _selection_data|
      compound_text = Gdk::Atom.intern("COMPOUND_TEXT")
      @clipboard.request_contents(compound_text) do |_clipboard, _selection_data|
        target_string = Gdk::Selection::TARGET_STRING
        @clipboard.request_contents(target_string) do |_clipboard, selection_data|
          received_text = selection_data.text
          loop.quit
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
      loop.quit
    end
    @clipboard.text = "hello"
    loop.run

    assert_equal("hello", received_text)
  end

  test "#request_image" do
    loop = GLib::MainLoop.new
    received_image = nil
    @clipboard.request_image do |_clipboard, image|
      received_image = image
      loop.quit
    end
    image = GdkPixbuf::Pixbuf.new(:file => fixture_path("gnome-logo-icon.png"))
    @clipboard.image = image
    loop.run

    assert_equal([image.width, image.height],
                 [received_image.width, received_image.height])
  end

  test "#request_targets" do
    loop = GLib::MainLoop.new
    received_atoms = nil
    @clipboard.request_targets do |_clipboard, atoms|
      received_atoms = atoms
      loop.quit
    end
    loop.run

    assert_nil(received_atoms)
  end

  test "#request_rich_text" do
    only_gtk_version(3, 12, 0)
    loop = GLib::MainLoop.new
    received_format = nil
    received_text = nil
    table = Gtk::TextTagTable.new
    buffer = Gtk::TextBuffer.new(table)
    buffer.register_deserialize_tagset(nil)
    @clipboard.request_rich_text(buffer) do |_clipboard, format, text|
      received_format = format
      received_text = text
      loop.quit
    end
    loop.run

    assert_equal([
                   "application/x-gtk-text-buffer-rich-text",
                   nil,
                 ],
                 [
                   received_format.name,
                   received_text,
                 ])
  end

  test "#request_uris" do
    loop = GLib::MainLoop.new
    received_uris = nil
    @clipboard.request_uris do |_clipboard, uris|
      received_uris = uris
      loop.quit
    end
    loop.run

    assert_nil(received_uris)
  end
end
