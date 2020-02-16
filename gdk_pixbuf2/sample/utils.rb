#!/usr/bin/env ruby
=begin
  utils.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.

  $Id: utils.rb,v 1.4 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk3'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

src =  GdkPixbuf::Pixbuf.new(:file => filename)

vbox = Gtk::Box.new(:vertical)

dst = src.add_alpha(true, 0, 0, 0)
vbox.pack_start(Gtk::Image.new(dst))

dst = GdkPixbuf::Pixbuf.new(:colorspace => :rgb,
                            :has_alpha => true,
                            :bits_per_sample => 8, 
                            :width => src.width + 20,
                            :height => src.height + 30)
dst.fill!(0xff000099) # Clears a pixbuf to the given RGBA value
src.copy_area(0, 0, src.width / 2, src.height / 2, dst, 10, 20)
vbox.pack_start(Gtk::Image.new(:pixbuf => dst))

dst = src.saturate_and_pixelate(0.3, true)
vbox.pack_start(Gtk::Image.new(:pixbuf => dst))

dst = src.fill!(0xff000099)
vbox.pack_start(Gtk::Image.new(:pixbuf => dst))

w = Gtk::Window.new.add(vbox)
w.signal_connect('delete-event') do
  Gtk.main_quit
end

w.show_all

Gtk.main
