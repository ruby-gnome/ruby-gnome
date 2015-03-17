#!/usr/bin/env ruby
=begin
  window.rb - Gtk::Window sample.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("Gtk::Window sample")
window.signal_connect("destroy"){Gtk.main_quit}

button = Gtk::Button.new(:label => "Hello World")
button.signal_connect("clicked") do
  puts "hello world"
  Gtk.main_quit
end
window.add(button)
window.show_all

Gtk.main
