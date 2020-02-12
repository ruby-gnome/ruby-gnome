#!/usr/bin/env ruby
=begin
  image.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "gtk3"

window = Gtk::Window.new("Image")
window.signal_connect("destroy") { Gtk.main_quit }
window.border_width = 10

button = Gtk::Button.new

label = Gtk::Label.new("Gtk::Image")
image = Gtk::Image.new(:file => "test.xpm")

box = Gtk::Box.new(:horizontal, 5)
box.add(image)
box.add(label)

button.add(box)

window.add(button)
window.show_all

Gtk.main
