=begin
  cursor.rb - Gdk::Cursor sample script.

  Copyright (C) 2001 MUTOH Masao<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME.

  $Date: 2002/05/19 12:29:42 $
  $Id: cursor.rb,v 1.1 2002/05/19 12:29:42 mutoh Exp $
=end

require 'gtk'

window = Gtk::Window.new
window.realize


button = Gtk::Button.new("Click!")

cursors = Gdk::Cursor::Constants.constants.sort - 
          ["LAST_CURSOR", "CURSOR_IS_PIXMAP","NUM_GLYPHS"]

cnt = 0
button.signal_connect(Gtk::Button::SIGNAL_CLICKED) do
  p cursors[cnt]
  cursor = eval("Gdk::Cursor::Constants::" + cursors[cnt])
  button.child.set_text(cnt.to_s + ":" + cursors[cnt])
  window.window.set_cursor(Gdk::Cursor.new(cursor))
  cnt += 1
  cnt = 0 if cnt == cursors.size
end
window.add(button)
window.set_usize(200,100).show_all

Gtk.main
