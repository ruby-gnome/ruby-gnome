=begin
  cursor.rb - Gdk::Cursor sample script.

  Copyright (C) 2001-2003 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: cursor.rb,v 1.6 2003/08/31 15:29:44 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
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
