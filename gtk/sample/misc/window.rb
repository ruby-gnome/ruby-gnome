#!/usr/bin/env ruby
=begin
  window.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: window.rb,v 1.6 2005/03/22 17:41:28 silicio Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.border_width = 10
button = Gtk::Button::new("Hello World")
button.signal_connect("clicked") do
  puts "hello world"
  Gtk.main_quit
end
window.add(button)
window.show_all
Gtk.main
