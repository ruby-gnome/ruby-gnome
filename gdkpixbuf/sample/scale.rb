#!/usr/local/bin/ruby
=begin
  scale.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: scale.rb,v 1.7 2003/01/17 19:20:45 mutoh Exp $
=end

require 'gtk2'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

Gtk.init

src =  Gdk::Pixbuf.new(filename)

dst = src.scale(300, 300, Gdk::Pixbuf::INTERP_HYPER)
pw = Gtk::Image.new(dst)
pw.show

w = Gtk::Window.new
w.signal_connect('delete-event') do
  Gtk.main_quit
end


w.add pw
w.show

Gtk.main
