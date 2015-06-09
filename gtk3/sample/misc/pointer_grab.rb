=begin
  pointer_grab.rb - Gdk::Window#pointer_grab/ungrab sample script.

  Copyright (C) 2001-2015 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "gtk3"

window = Gtk::Window.new("Pointer grab/ungrab sample")

cursor = Gdk::Cursor.new(:watch)
button1 = Gtk::Button.new(:label => "Grab Window!")
button2 = Gtk::Button.new(:label => "Ungrab Window!")

button1.signal_connect("clicked") do
  Gdk.pointer_grab(window.window, true, :button_press_mask,
                   window.window, cursor, Gdk::CURRENT_TIME)
  p Gdk.pointer_is_grabbed?
end

button2.signal_connect("clicked") do
  Gdk.pointer_ungrab(Gdk::CURRENT_TIME)
  p Gdk.pointer_is_grabbed?
end

vbox = Gtk::Box.new(:vertical, 0)
vbox.add(button1)
vbox.add(button2)

window.add(vbox)
window.set_default_size(200, 100)
window.show_all
window.signal_connect("destroy") { Gtk.main_quit }

Gtk.main
