#!/usr/local/bin/ruby
=begin
  xpm.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: xpm.rb,v 1.1 2002/11/20 15:36:01 mutoh Exp $
=end

require 'gdk_pixbuf2'

r_xpm = [
      "10 10 3 1", 
      "   c None", 
      ".  c #FE0B0B", 
      "+  c #FFFFFF", 
      "+.......++",   
      "+..    ..+",
      "+..    ..+",
      "+..   ...+",
      "+.......++",
      "+.....++++",
      "+..++..+++",
      "+..++...++",
      "+..+++...+",
      "+..++++..+"]     

Gtk.init

w = Gtk::Window.new
w.signal_connect('delete-event') do
  Gtk.main_quit
end

src =  Gdk::Pixbuf.new(r_xpm)
dst = src.scale(300, 300, Gdk::Pixbuf::INTERP_HYPER)

w.add(Gtk::Image.new(dst))
w.show_all

Gtk.main
