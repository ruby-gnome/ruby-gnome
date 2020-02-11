=begin
  cursor.rb - Gdk::Cursor sample script.

  Copyright (C) 2001-2006 Masao Mutoh
  Copyright (c) 2002-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "gtk3"

window = Gtk::Window.new("Gdk::Cursor sample")
window.signal_connect('destroy') {Gtk.main_quit}
window.realize

button = Gtk::Button.new(:label => "Click!")
button.use_underline = false

cursors = Gdk::CursorType.values -
          [Gdk::CursorType::CURSOR_IS_PIXMAP] -
          [Gdk::CursorType::LAST_CURSOR]
cursors = cursors.cycle

button.signal_connect('clicked') do
  cursor = cursors.next
  p cursor
  button.set_label(cursor.inspect)
  window.window.set_cursor(Gdk::Cursor.new(cursor))
end

window.add(button)
window.set_default_size(400, 100)
window.show_all

Gtk.main
