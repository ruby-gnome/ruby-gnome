#!/usr/bin/env ruby
=begin
  scale.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: scale.rb,v 1.10 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk2'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

vbox = Gtk::VBox.new

src =  Gdk::Pixbuf.new(filename)
vbox.add(Gtk::Image.new(src))

dst = src.scale(200, 200, Gdk::Pixbuf::INTERP_NEAREST)
dst.scale!(src, 60, 60, 90, 90, -50, 50, 6, 3)
vbox.add(Gtk::Image.new(dst))

dst2 = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8, 200, 200)
dst2.scale!(src, 0, 0, 100, 100, 0, 0, 1.5, 1.5)

vbox.add(Gtk::Image.new(dst2))

dst3 = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8, 200, 200)
dst3.scale!(src, 0, 0, 200, 200, 0, 0, 5, 3,  Gdk::Pixbuf::INTERP_HYPER)
vbox.add(Gtk::Image.new(dst3))

window = Gtk::Window.new
window.signal_connect('delete-event') do
  Gtk.main_quit
end

window.add(vbox).show_all

Gtk.main

