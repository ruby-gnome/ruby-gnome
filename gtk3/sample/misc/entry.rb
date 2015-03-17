#!/usr/bin/env ruby
=begin
  entry.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("entry")
window.border_width = 0

box1 = Gtk::Box.new(:vertical, 10)
window.add(box1)

entry = Gtk::Entry.new

box1.add(entry)
box1.add(Gtk::Separator.new(:horizontal))

button = Gtk::Button.new(:label => "close")
button.signal_connect("clicked") do
  Gtk.main_quit
end
box1.add(button)

window.show_all

entry.text = "hello world"

Gtk.main
