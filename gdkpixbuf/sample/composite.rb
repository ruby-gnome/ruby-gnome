#!/usr/bin/env ruby
=begin
  composite.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: composite.rb,v 1.6 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk2'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

src =  Gdk::Pixbuf.new(filename)

vbox = Gtk::VBox.new

dst = src.composite(100, 100, Gdk::Pixbuf::INTERP_HYPER,
		200, 32, 0xFF0000, 0x00FF00)
vbox.pack_start(Gtk::Image.new(dst))

dst = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8, 200, 200)
dst.composite!(src, 0, 0, 200, 200, 0, 0, 1.8, 1.8,
              Gdk::Pixbuf::INTERP_HYPER, 200)
vbox.pack_start(Gtk::Image.new(dst))

dst = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8, 200, 200)
dst.composite!(src, 10, 10, 180, 180, 15, 15, 3, 2, 
              Gdk::Pixbuf::INTERP_BILINEAR, 200, 100, 100, 16, 
              0x999999, 0xdddddd)
vbox.pack_start(Gtk::Image.new(dst))

window = Gtk::Window.new.add(vbox)
window.signal_connect('delete-event') do
  Gtk.main_quit
end

window.show_all

Gtk.main
