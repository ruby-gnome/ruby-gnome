#!/usr/bin/env ruby
=begin
  helloworld.rb - Ruby/GTK2 first sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

button = Gtk::Button.new(:label => "Hello World")
button.signal_connect("clicked") {
  puts "Hello World"
}

window = Gtk::Window.new("Hello World sample")
window.signal_connect("delete_event") {
  puts "delete event occurred"
  false
}

window.signal_connect("destroy") {
  puts "destroy event occurred"
  Gtk.main_quit
}

window.border_width = 10
window.add(button)
window.show_all

Gtk.main

