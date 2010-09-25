=begin
  pointer_grab.rb - Gdk::Window#pointer_grab/ungrab sample script.

  Copyright (C) 2001-2006 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME.

  $Date: 2006/06/17 13:18:12 $
  $Id: pointer_grab.rb,v 1.5 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Pointer grab/ungrab sample")

cursor = Gdk::Cursor.new(Gdk::Cursor::WATCH)
button1 = Gtk::Button.new("Grab Window!")
button2 = Gtk::Button.new("Ungrab Window!")

button1.signal_connect('clicked') do 
  Gdk.pointer_grab(window.window, true, Gdk::Event::BUTTON_PRESS_MASK, 
			     window.window, cursor, Gdk::Event::CURRENT_TIME)
  p Gdk.pointer_is_grabbed?
end

button2.signal_connect('clicked') do
  Gdk.pointer_ungrab(Gdk::Event::CURRENT_TIME)
  p Gdk.pointer_is_grabbed?
end

window.add(Gtk::VBox.new.add(button1).add(button2))
window.set_default_size(200,100).show_all
window.signal_connect("destroy"){Gtk.main_quit}

Gtk.main
