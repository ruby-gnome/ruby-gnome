#!/usr/local/bin/ruby
=begin
  composite.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: composite.rb,v 1.1 2002/11/21 16:49:40 mutoh Exp $
=end

require 'gdk_pixbuf2'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

Gtk.init

src =  Gdk::Pixbuf.new(filename)

vbox = Gtk::VBox.new

dst = src.composite(100, 100, Gdk::Pixbuf::INTERP_HYPER,
		200, 32, 0xFF0000, 0x00FF00)
vbox.pack_start(Gtk::Image.new(dst))

dst = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8, 200, 200)
src.composite(dst, 0, 0, 200, 200, 0, 0, 1.8, 1.8,
              Gdk::Pixbuf::INTERP_HYPER, 200)
vbox.pack_start(Gtk::Image.new(dst))

dst = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8, 200, 200)
src.composite(dst, 10, 10, 180, 180, 15, 15, 3, 2, 
              Gdk::Pixbuf::INTERP_BILINEAR, 200, 100, 100, 16, 
              0x999999, 0xdddddd)
vbox.pack_start(Gtk::Image.new(dst))

w = Gtk::Window.new.add(vbox)
w.signal_connect('delete-event') do
  Gtk.main_quit
end

w.show_all

Gtk.main
