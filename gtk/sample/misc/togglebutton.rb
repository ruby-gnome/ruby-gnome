#!/usr/local/bin/ruby
=begin
  togglebutton.rb - Ruby/GTK sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: togglebutton.rb,v 1.2 2002/11/05 10:39:11 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.set_title("toggle buttons")
window.set_border_width(0)

box1 = Gtk::VBox.new(false, 0)
window.add(box1)

box2 = Gtk::VBox.new(false, 10)
box2.set_border_width(10)
box1.pack_start(box2, true, true, 0)

button = Gtk::ToggleButton.new("button1")
box2.pack_start(button, true, true, 0)

button = Gtk::ToggleButton.new("button2")
box2.pack_start(button, true, true, 0)

button = Gtk::ToggleButton.new("button3")
box2.pack_start(button, true, true, 0)

separator = Gtk::HSeparator.new()
box1.pack_start(separator, false, true, 0)

box2 = Gtk::VBox.new(false, 10)
box2.set_border_width(10)
box1.pack_start(box2, false, true, 0)

close = Gtk::Button.new("close")
close.signal_connect("clicked") do
  window.destroy
  exit
end
box2.pack_start(close, true, true, 0)
close.set_flags(Gtk::Widget::CAN_DEFAULT);
close.grab_default
close.show

window.show_all

Gtk.main
