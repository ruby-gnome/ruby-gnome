#!/usr/local/bin/ruby
=begin
  scale.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: scale.rb,v 1.4 2002/11/19 13:07:24 mutoh Exp $
=end

require 'gdk_pixbuf2'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

Gtk.init

src =  Gdk::Pixbuf.new(filename)

pixmap, mask = src.scale(300, 300, 
			 Gdk::Pixbuf::INTERP_HYPER).render_pixmap_and_mask
pw = Gtk::Image.new(pixmap, mask)
pw.show

w = Gtk::Window.new
w.signal_connect('delete-event') do
  Gtk.main_quit
end


w.add pw
w.show

Gtk.main
