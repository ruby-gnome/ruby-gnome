#!/usr/local/bin/ruby
=begin
  helloworld.rb - Ruby/GTK first sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: helloworld.rb,v 1.2 2002/11/05 10:39:11 mutoh Exp $
=end

require 'gtk2'

Gtk.init

button = Gtk::Button.new("Hello World")
button.signal_connect("clicked") {
  print ("Hello World\n")
}

window = Gtk::Window.new
window.signal_connect("delete_event") {
  print ("delete event occurred\n")
  #true
  false
}

window.signal_connect("destroy") {
  print ("destroy event occurred\n")
  exit
}

window.border_width = 10
window.add(button)
window.show_all

Gtk.main

