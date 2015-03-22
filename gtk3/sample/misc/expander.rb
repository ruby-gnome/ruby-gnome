#!/usr/bin/env ruby
=begin
  expander.rb - Ruby/GTK2 sample script.

  Copyright (c) 2004-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

expander = Gtk::Expander.new("Expander Sample")
expander.expanded = true

button = Gtk::Button.new(:label => "Hide this button.")
button.signal_connect("clicked") do
  expander.expanded = false
end

button.set_size_request(200, 200)
expander.add(button)

win = Gtk::Window.new
win.add(expander)
win.show_all
win.signal_connect("destroy"){Gtk.main_quit}

Gtk.main

