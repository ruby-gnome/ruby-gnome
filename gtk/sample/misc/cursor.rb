=begin
  cursor.rb - Gdk::Cursor sample script.

  Copyright (C) 2001,2002 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: cursor.rb,v 1.2 2002/11/05 10:39:10 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.realize

button = Gtk::Button.new("Click!")

cursors = (Gdk::Cursor.constants - Gdk::Cursor.superclass.constants).sort{|a, b| 
  Gdk::Cursor.const_get(a) <=> Gdk::Cursor.const_get(b)}

cnt = 0
button.signal_connect('clicked') do
  cursor = eval("Gdk::Cursor::" + cursors[cnt])
  button.child.set_text("Gdk::Cursor::" + cursors[cnt] + ", value = " + cursor.to_s)
  window.window.set_cursor(Gdk::Cursor.new(cursor))
  cnt += 1
  cnt = 0 if cnt == cursors.size
end
window.add(button)
window.set_default_size(400,100).show_all

Gtk.main
