#!/usr/local/bin/ruby
=begin
  frame.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: frame.rb,v 1.6 2003/04/01 16:26:33 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.signal_connect("destroy") do
  Gtk.main_quit
end
window.set_title("buttons")
window.set_border_width(0)

box1 = Gtk::VBox.new(false, 0)
window.add box1

box2 = Gtk::HBox.new(false, 5)
box2.set_border_width 10
box1.pack_start box2, true, true, 0

label = Gtk::Label.new("Hello World")
frame = Gtk::Frame.new("Frame 1")
box2.pack_start frame, true, true, 0

box3 = Gtk::VBox.new(false, 5)
box3.set_border_width 5
frame.add box3

button = Gtk::Button.new("switch")
button.signal_connect("clicked") do
  label.reparent box3
end
box3.pack_start button, false, true, 0
box3.pack_start label, false, true, 0

frame = Gtk::Frame.new("Frame 2")
box2.pack_start frame, true, true, 0

box4 = Gtk::VBox.new(false, 5)
box4.set_border_width 5
frame.add box4

button = Gtk::Button.new("switch")
button.signal_connect("clicked") do
  label.reparent box4
end
box4.pack_start button, false, true, 0

separator = Gtk::HSeparator.new
box1.pack_start(separator, false, true, 0)

box2 = Gtk::HBox.new(false, 10)
box2.set_border_width(10)
box1.pack_start(box2, false, true, 0)

button = Gtk::Button.new("close")
button.signal_connect("clicked") do
	Gtk.main_quit
end
box2.pack_start(button, true, true, 0)
button.set_flags(Gtk::Widget::CAN_DEFAULT)
button.grab_default

window.show_all

Gtk.main
