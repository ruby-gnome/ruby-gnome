=begin
  cursor.rb - Gdk::Cursor sample script.

  Copyright (C) 2001-2003 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: cursor.rb,v 1.5 2003/08/29 19:14:53 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.realize

button = Gtk::Button.new("Click!")
button.use_underline = false

cursors = (Gdk::Cursor.constants - Gdk::Cursor.superclass.constants -  
           ["Type", "CURSOR_IS_PIXMAP"]).sort{|a, b| 
  Gdk::Cursor.const_get(a) <=> Gdk::Cursor.const_get(b)
}

cnt = 0
button.signal_connect('clicked') do
  cursor = eval("Gdk::Cursor::" + cursors[cnt])
  button.set_label("Gdk::Cursor::" + cursors[cnt] + ", value = " + cursor.inspect)
  window.window.set_cursor(Gdk::Cursor.new(cursor))
  cnt += 1
  cnt = 0 if cnt == cursors.size
end
window.add(button)
window.set_default_size(400,100).show_all

Gtk.main
