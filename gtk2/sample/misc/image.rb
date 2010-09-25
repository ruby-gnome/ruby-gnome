#!/usr/bin/env ruby
=begin
  image.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: image.rb,v 1.9 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Image")
window.signal_connect("destroy") do
  Gtk.main_quit
end
window.border_width = 0

box1 = Gtk::VBox.new(false, 10)
box1.border_width = 10

button = Gtk::Button.new
box1.add(button)

label = Gtk::Label.new("Gtk::Image\ntest")
image = Gtk::Image.new("test.xpm")

box2 = Gtk::HBox.new(false, 5).add(image).add(label)

button.add(box2)

box1.add(Gtk::HSeparator.new)

button = Gtk::Button.new("close")
button.signal_connect("clicked") do
  Gtk.main_quit
end
box1.add(button)

window.add(box1).show_all

Gtk.main
