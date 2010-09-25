#!/usr/bin/env ruby
=begin
  radiobutton.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: radiobutton.rb,v 1.10 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Gtk::RadioButton sample")
window.signal_connect("destroy"){Gtk.main_quit}

box1 = Gtk::VBox.new
window.add(box1)

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.add(box2)

button1 = Gtk::RadioButton.new("_button1")
button2 = Gtk::RadioButton.new(button1, "_button2", false)
button3 = Gtk::RadioButton.new(button1, Gtk::Stock::QUIT)

box2.add(button1).add(button2).add(button3)

box1.add(Gtk::HSeparator.new)

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.add(box2)

close = Gtk::Button.new("close")
close.signal_connect("clicked") do
  Gtk.main_quit
end
box2.add(close)

close.can_default = true
close.grab_default

window.signal_connect("destroy"){Gtk.main_quit}
window.show_all

Gtk.main
