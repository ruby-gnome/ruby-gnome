=begin
  inline.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2004-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: inline.rb,v 1.2 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk2'

pixbuf = Gdk::Pixbuf.new("gnome-foot.png")

#pixbuf = Gdk::Pixbuf.new(ARGV[0])

# Create Gdk::Pixbuf from pixels(String).
pixbuf2 = Gdk::Pixbuf.new(pixbuf.pixels, Gdk::Pixbuf::COLORSPACE_RGB, 
                          true, pixbuf.bits_per_sample, 
                          pixbuf.width, pixbuf.height, 
                          pixbuf.rowstride)

# Create Gdk::Pixbuf from an array of numbers 
# which created by Gdk::Pixdata#serialize.
pixdata = Gdk::Pixdata.from_pixbuf(pixbuf, false)
data = pixdata.serialize
pixbuf3 = Gdk::Pixbuf.new(data, false)

# Create Gdk::Pixbuf from Gdk::Pixbuf.
pixbuf4 = Gdk::Pixbuf.new(pixbuf, 10, 10, 20, 20)

vbox = Gtk::VBox.new
vbox.add(Gtk::Image.new(pixbuf))
vbox.add(Gtk::Image.new(pixbuf2))
vbox.add(Gtk::Image.new(pixbuf3))
vbox.add(Gtk::Image.new(pixbuf4))
Gtk::Window.new.add(vbox).show_all
Gtk.main
