#!/usr/local/bin/ruby
=begin
  button2.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: button2.rb,v 1.4 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk3'

window = Gtk::Window.new("Gtk::Button sample")

button1 = Gtk::Button.new(:mnemonic => "_HELLO")
button2 = Gtk::Button.new(:label => "_HELLO")
button3 = Gtk::Button.new(:stock_id => Gtk::Stock::QUIT)  

button3.signal_connect("clicked"){ Gtk.main_quit }

box = Gtk::Box.new(:vertical)
box.add(button1)
box.add(button2)
box.add(button3)

window.add(box).set_default_size(100,100).show_all

Gtk.main
