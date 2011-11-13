=begin
  cursor.rb - Gdk::Cursor sample script.

  Copyright (C) 2001-2006 Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: cursor.rb,v 1.7 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Gdk::Cursor sample")
window.realize

button = Gtk::Button.new("Click!")
button.use_underline = false

cursors = Gdk::Cursor::Type.values - [Gdk::Cursor::CURSOR_IS_PIXMAP]

cnt = 0
button.signal_connect('clicked') do
  cursor = cursors[cnt]
p cursor.inspect
  button.set_label(cursor.inspect)
  window.window.set_cursor(Gdk::Cursor.new(cursor))
  cnt += 1
  cnt = 0 if cnt == cursors.size
end
window.add(button)
window.set_default_size(400,100).show_all

Gtk.main
