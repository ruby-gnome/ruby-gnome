=begin
  keyboard_grab.rb - Gdk::Window#keyboard_grab/ungrab sample script.

  Copyright (C) 2001 MUTOH Masao<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME.

  $Date: 2002/10/31 17:08:29 $
  $Id: keyboard_grab.rb,v 1.3 2002/10/31 17:08:29 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new

button1 = Gtk::Button.new("Grab Keyboard!")
button2 = Gtk::Button.new("Ungrab Keyboard!")

button1.signal_connect('clicked') do
  Gdk.keyboard_grab(window.window, true, Gdk::Event::CURRENT_TIME)
end

button2.signal_connect('clicked') do
  Gdk.keyboard_ungrab(Gdk::Event::CURRENT_TIME)
end

window.add(Gtk::VBox.new.add(button1).add(button2))
window.set_default_size(200,100).show_all

Gtk.main
