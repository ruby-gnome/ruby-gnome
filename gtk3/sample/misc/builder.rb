#!/usr/bin/env ruby
=begin
  builder.rb - Ruby/GTK version of gtk+/examples/builder.c.
  https://git.gnome.org/browse/gtk+/tree/examples/builder.c?h=gtk-3-16
  Copyright (c) 2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

ui_file = File.expand_path("builder.ui", __dir__)
builder = Gtk::Builder.new
builder.add_from_file(ui_file)

win = builder.get_object("window")
win.signal_connect("delete-event") { Gtk.main_quit }

button = builder.get_object("button1")
button.signal_connect "clicked" do
  puts "Hello World!"
end

button = builder.get_object("button2")
button.signal_connect "clicked" do
  puts "Hello World!"
end

button = builder.get_object("quit")
button.signal_connect("clicked") { Gtk.main_quit }

Gtk.main
