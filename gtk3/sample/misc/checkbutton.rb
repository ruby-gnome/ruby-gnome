#!/usr/bin/env ruby
=begin
  checkbutton.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: checkbutton.rb,v 1.10 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("check buttons")
window.border_width = 0

box1 = Gtk::VBox.new
window.add(box1)

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2)

button1 = Gtk::CheckButton.new("_button1")
button2 = Gtk::CheckButton.new("_button2", false)
button3 = Gtk::CheckButton.new(Gtk::Stock::QUIT)
box2.add(button1)
box2.add(button2)
box2.add(button3)

box1.pack_start(Gtk::HSeparator.new)

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2)

close = Gtk::Button.new("close")
close.signal_connect("clicked") do
  Gtk.main_quit
end
box2.pack_start(close)
close.can_default = true
close.grab_default

window.show_all

Gtk.main
