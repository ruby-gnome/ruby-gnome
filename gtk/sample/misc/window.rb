#!/usr/local/bin/ruby
=begin
  window.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: window.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
window.border_width = 10
button = Gtk::Button::new("Hello World")
button.signal_connect("clicked") do
  print "hello world\n"
  exit
end
window.add(button)
button.show
window.show
Gtk.main
