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

src =  GdkPixbuf::Pixbuf.new(:file => filename)

vbox = Gtk::VBox.new

dst = src.composite_color_simple(100, 100, :hyper,
		200, 32, 0xFF0000, 0x00FF00)
vbox.pack_start(Gtk::Image.new(dst))

dst = GdkPixbuf::Pixbuf.new(:colorspace => :rgb,
                            :has_alpha => true,
                            :bits_per_sample => 8,
                            :width =>200,
                            :height => 200)
dst.composite!(src, 0, 0, 200, 200, 0, 0, 1.8, 1.8,
              :hyper, 200)
vbox.pack_start(Gtk::Image.new(dst))

dst = GdkPixbuf::Pixbuf.new(:colorspace => :rgb,
                            :has_alpha => true,
                            :bits_per_sample => 8,
                            :width =>200,
                            :height => 200)
src.composite_color(dst, 10, 10, 180, 180, 15, 15, 3, 2,
              :bilinear, 200, 100, 100, 16,
              0x999999, 0xdddddd)
vbox.pack_start(Gtk::Image.new(dst))

window = Gtk::Window.new.add(vbox)
window.signal_connect('delete-event') do
  Gtk.main_quit
end

window.show_all

Gtk.main
