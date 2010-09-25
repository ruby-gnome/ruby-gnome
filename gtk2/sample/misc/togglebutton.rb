#!/usr/bin/env ruby
=begin
  togglebutton.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: togglebutton.rb,v 1.10 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Gtk::ToggleButton sample")
window.border_width = 10

box = Gtk::VBox.new(false, 10)
window.add(box)

button1 = Gtk::ToggleButton.new("_button1")
button2 = Gtk::ToggleButton.new("_button2", false)
button3 = Gtk::ToggleButton.new(Gtk::Stock::QUIT)
box.add(button1).add(button2).add(button3)

box.pack_start(Gtk::HSeparator.new)

close = Gtk::Button.new("close")
close.signal_connect("clicked") do
  Gtk.main_quit
end
box.add(close)

window.show_all

Gtk.main
