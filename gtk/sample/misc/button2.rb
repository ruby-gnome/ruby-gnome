#!/usr/local/bin/ruby
=begin
  button2.rb - Ruby/GTK sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: button2.rb,v 1.1 2002/11/30 17:40:47 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new

button1 = Gtk::Button.new("_HELLO")
button2 = Gtk::Button.new("_HELLO", false)
button3 = Gtk::Button.new(Gtk::Stock::QUIT)  

box = Gtk::VBox.new
box.add(button1)
box.add(button2)
box.add(button3)

window.add(box).set_default_size(100,100).show_all

Gtk.main
