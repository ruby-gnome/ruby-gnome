#!/usr/local/bin/ruby
=begin
  radiobutton.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: radiobutton.rb,v 1.7 2003/04/01 16:26:34 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.set_title("radio buttons")
window.set_border_width(0)

box1 = Gtk::VBox.new(false, 0)
window.add(box1)

box2 = Gtk::VBox.new(false, 10)
box2.set_border_width 10
box1.pack_start(box2, true, true, 0)

button1 = Gtk::RadioButton.new("_button1")
button2 = Gtk::RadioButton.new(button1, "_button2", false)
button3 = Gtk::RadioButton.new(button1, Gtk::Stock::QUIT)
box2.add(button1).add(button2).add(button3)

separator = Gtk::HSeparator.new
box1.pack_start(separator, false, true, 0)

box2 = Gtk::VBox.new(false, 10)
box2.set_border_width(10)
box1.pack_start(box2, false, true, 0)

close = Gtk::Button.new("close")
close.signal_connect("clicked") do
	Gtk.main_quit
end
box2.pack_start(close, true, true, 0)
close.set_flags(Gtk::Widget::CAN_DEFAULT)
close.grab_default

window.show_all

Gtk.main
