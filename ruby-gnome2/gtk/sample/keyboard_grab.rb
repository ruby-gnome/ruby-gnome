=begin
  keyboard_grab.rb - Gdk::Window#keyboard_grab/ungrab sample script.

  Copyright (C) 2001 MUTOH Masao<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME.

  $Date: 2002/05/19 12:29:42 $
  $Id: keyboard_grab.rb,v 1.1 2002/05/19 12:29:42 mutoh Exp $
=end

require 'gtk'

window = Gtk::Window.new

button1 = Gtk::Button.new("Grab Keyboard!")
button2 = Gtk::Button.new("Ungrab Keyboard!")

button1.signal_connect(Gtk::Button::SIGNAL_CLICKED) do
  window.window.keyboard_grab(true, Gdk::CURRENT_TIME)
end

button2.signal_connect(Gtk::Button::SIGNAL_CLICKED) do
  window.window.keyboard_ungrab(Gdk::CURRENT_TIME)
end

window.add(Gtk::VBox.new.add(button1).add(button2))
window.set_usize(200,100).show_all

Gtk.main
