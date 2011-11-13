=begin
  keyboard_grab.rb - Gdk::Window#keyboard_grab/ungrab sample script.

  Copyright (C) 2001-2006 Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME.

  $Date: 2006/06/17 13:18:12 $
  $Id: keyboard_grab.rb,v 1.5 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Keyboard Grab/Ungrab sample")

button1 = Gtk::Button.new("Grab Keyboard!")
button2 = Gtk::Button.new("Ungrab Keyboard!")

button1.signal_connect('clicked') do
  Gdk.keyboard_grab(window.window, true, Gdk::Event::CURRENT_TIME)
end

button2.signal_connect('clicked') do
  Gdk.keyboard_ungrab(Gdk::Event::CURRENT_TIME)
end

window.add(Gtk::VBox.new.add(button1).add(button2))
window.set_default_size(200,100).show_all.signal_connect("destroy"){Gtk.main_quit}

Gtk.main
