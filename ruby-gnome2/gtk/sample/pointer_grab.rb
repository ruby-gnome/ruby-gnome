=begin
  pointer_grab.rb - Gdk::Window#pointer_grab/ungrab sample script.

  Copyright (C) 2001 MUTOH Masao<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME.

  $Date: 2002/05/19 12:29:42 $
  $Id: pointer_grab.rb,v 1.1 2002/05/19 12:29:42 mutoh Exp $
=end

require 'gtk'

window = Gtk::Window.new

cursor = Gdk::Cursor.new(Gdk::Cursor::WATCH)
button1 = Gtk::Button.new("Grab Window!")
button2 = Gtk::Button.new("Ungrab Window!")

button1.signal_connect(Gtk::Button::SIGNAL_CLICKED) do 
  window.window.pointer_grab(true, Gdk::BUTTON_PRESS_MASK, 
			     window.window, cursor, Gdk::CURRENT_TIME)
  p window.window.pointer_is_grabbed?
end

button2.signal_connect(Gtk::Button::SIGNAL_CLICKED) do
  window.window.pointer_ungrab(Gdk::CURRENT_TIME)
  p window.window.pointer_is_grabbed?
end

window.add(Gtk::VBox.new.add(button1).add(button2))
window.set_usize(200,100).show_all

Gtk.main
