#!/usr/bin/env ruby
=begin
  frame.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: frame.rb,v 1.9 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("buttons")
window.signal_connect("destroy") do
  Gtk.main_quit
end
window.border_width = 0

box1 = Gtk::VBox.new(false, 0)
window.add(box1)

box2 = Gtk::HBox.new(false, 5)
box2.border_width = 10
box1.pack_start(box2, true, true, 0)

label = Gtk::Label.new("Hello World")
frame = Gtk::Frame.new("Frame 1")
box2.pack_start(frame, true, true, 0)

box3 = Gtk::VBox.new(false, 5)
box3.border_width = 5
frame.add(box3)

button = Gtk::Button.new("switch")
button.signal_connect("clicked") do
  label.reparent(box3)
end
box3.pack_start(button, false, true, 0)
box3.pack_start(label, false, true, 0)

frame = Gtk::Frame.new("Frame 2")
box2.pack_start(frame, true, true, 0)

box4 = Gtk::VBox.new(false, 5)
box4.border_width = 5
frame.add(box4)

button = Gtk::Button.new("switch")
button.signal_connect("clicked") do
  label.reparent(box4)
end
box4.pack_start(button, false, true, 0)

separator = Gtk::HSeparator.new
box1.pack_start(separator, false, true, 0)

box2 = Gtk::HBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2, false, true, 0)

button = Gtk::Button.new("close")
button.signal_connect("clicked") do
  Gtk.main_quit
end
box2.pack_start(button)

window.show_all

Gtk.main
