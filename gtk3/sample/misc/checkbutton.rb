#!/usr/bin/env ruby
=begin
  checkbutton.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("check buttons")
window.border_width = 0
window.signal_connect("delete_event") do
  Gtk.main_quit
end

box1 = Gtk::Box.new(:vertical)
window.add(box1)

box2 = Gtk::Box.new(:vertical, 10)
box2.border_width = 10
box1.pack_start(box2)

button1 = Gtk::CheckButton.new("_button1")
button1.set_use_underline(true)
button2 = Gtk::CheckButton.new("_button2")
button3 = Gtk::CheckButton.new(Gtk::Stock::QUIT)
box2.add(button1)
box2.add(button2)
box2.add(button3)

box1.pack_start(Gtk::Separator.new(:horizontal))

box2 = Gtk::Box.new(:vertical, 10)
box2.border_width = 10
box1.pack_start(box2)

close = Gtk::Button.new(:label => "close")
close.signal_connect("clicked") do
  Gtk.main_quit
end
box2.pack_start(close)
close.can_default = true
close.grab_default

window.show_all

Gtk.main
