#!/usr/local/bin/ruby
=begin
  image.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: image.rb,v 1.6 2003/04/01 16:26:34 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.signal_connect("destroy") do
  Gtk.main_quit
end
window.set_title("pixmap")
window.set_border_width(0)
window.realize

box1 = Gtk::VBox.new(false, 0)
window.add box1

box2 = Gtk::HBox.new(false, 10)
box2.set_border_width 10
box1.pack_start box2, true, true, 0

button = Gtk::Button.new
box2.pack_start button, false, false, 0

label = Gtk::Label.new("Gtk::Image\ntest")
box3 = Gtk::HBox.new(false, 0)
box3.set_border_width 2
box3.add Gtk::Image.new("test.xpm")
box3.add label
button.add box3

separator = Gtk::HSeparator.new
box1.pack_start(separator, false, true, 0)

box2 = Gtk::HBox.new(false, 10)
box2.set_border_width(10)
box1.pack_start(box2, false, true, 0)

button = Gtk::Button.new("close")
button.signal_connect("clicked") do
	Gtk.main_quit
end
box2.pack_start(button, true, true, 0)
button.set_flags(Gtk::Widget::CAN_DEFAULT)
button.grab_default

window.show_all

Gtk.main
