#!/usr/bin/env ruby
=begin
  togglebutton.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("Gtk::ToggleButton sample")
window.border_width = 10

box = Gtk::Box.new(:vertical, 10)
window.add(box)

button1 = Gtk::ToggleButton.new(:label => "_button1", :use_underline => true)
button2 = Gtk::ToggleButton.new(:label => "_button2", :use_underline => false)
box.add(button1)
box.add(button2)

box.pack_start(Gtk::Separator.new(:horizontal))

close = Gtk::Button.new(:label => "close")
close.signal_connect("clicked") do
  Gtk.main_quit
end
box.add(close)

window.show_all

Gtk.main
