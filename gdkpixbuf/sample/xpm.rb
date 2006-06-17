#!/usr/bin/env ruby
=begin
  xpm.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: xpm.rb,v 1.5 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk2'

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

w = Gtk::Window.new
w.signal_connect('delete-event') do
  Gtk.main_quit
end

src =  Gdk::Pixbuf.new(r_xpm)
dst = src.scale(300, 300, Gdk::Pixbuf::INTERP_HYPER)

w.add(Gtk::Image.new(dst))
w.show_all

Gtk.main
