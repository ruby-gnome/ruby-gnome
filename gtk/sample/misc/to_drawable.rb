#!/usr/bin/env ruby
=begin
  to_drawable.rb - Gdk::Pixbuf(GDK methods) sample script.

  Copyright (c) 2002-2005 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: to_drawable.rb,v 1.5 2005/07/14 17:01:49 mutoh Exp $
=end

require 'gtk2'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

Gtk.init

pixbuf = Gdk::Pixbuf.new(filename)

w = Gtk::Window.new
w.realize
w.signal_connect('delete-event') do
  Gtk.main_quit
end

d = Gtk::DrawingArea.new
gc = Gdk::GC.new(w.window)
d.signal_connect('expose-event') do |w, e|
  unless e.count > 0
    width = w.allocation.width
    height = w.allocation.height
    pixbuf = pixbuf.scale(width, height)
    d.window.draw_pixbuf(gc, pixbuf, 0, 0, 0, 0,
			 width, height, Gdk::RGB::DITHER_NORMAL, 0, 0)
  end
  true
end
w.add(d)
w.show_all

Gtk.main
