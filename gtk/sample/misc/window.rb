#!/usr/bin/env ruby
=begin
  window.rb - Gtk::Window sample.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: window.rb,v 1.9 2006/10/21 16:58:00 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Gtk::Window sample")
window.signal_connect("destroy"){Gtk.main_quit}

button = Gtk::Button.new("Hello World")
button.signal_connect("clicked") do
  puts "hello world"
  Gtk.main_quit
end
window.add(button)
window.show_all

Gtk.main
