#!/usr/local/bin/ruby
=begin
  scale.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: scale.rb,v 1.5 2002/11/20 15:36:00 mutoh Exp $
=end

require 'gdk_pixbuf2'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

Gtk.init

src =  Gdk::Pixbuf.new(filename)

dst = src.scale(300, 300, 
			 Gdk::Pixbuf::INTERP_HYPER)
pw = Gtk::Image.new(dst)
pw.show

w = Gtk::Window.new
w.signal_connect('delete-event') do
  Gtk.main_quit
end


w.add pw
w.show

Gtk.main
