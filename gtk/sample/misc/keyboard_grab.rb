=begin
  keyboard_grab.rb - Gdk::Window#keyboard_grab/ungrab sample script.

  Copyright (C) 2001-2005 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME.

  $Date: 2005/07/14 17:01:49 $
  $Id: keyboard_grab.rb,v 1.4 2005/07/14 17:01:49 mutoh Exp $
=end

require 'gtk2'

Gtk.init

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
window.set_default_size(200,100).show_all.signal_connect("destroy"){Gtk.main_quit}

Gtk.main
