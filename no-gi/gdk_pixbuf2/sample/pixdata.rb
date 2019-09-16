=begin
  pixdata.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2004-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: pixdata.rb,v 1.2 2006/06/17 14:38:08 mutoh Exp $
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

pixbuf = Gdk::Pixbuf.new(r_xpm)
pixdata = Gdk::Pixdata.from_pixbuf(pixbuf, false)

#Serialize data
p data = pixdata.serialize

#Deserialize data
pixdata = Gdk::Pixdata.deserialize(data)
pixbuf2 = pixdata.to_pixbuf(false)

dst = pixbuf2.scale(300, 300, Gdk::Pixbuf::INTERP_HYPER)
Gtk::Window.new.add(Gtk::Image.new(dst)).show_all
Gtk.main
