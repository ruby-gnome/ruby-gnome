#!/usr/bin/env ruby
=begin
  utils.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: utils.rb,v 1.4 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk2'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

src =  Gdk::Pixbuf.new(filename)

vbox = Gtk::VBox.new

dst = src.add_alpha(true, 0, 0, 0)
vbox.pack_start(Gtk::Image.new(dst))

dst = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8, 
                      src.width + 20, src.height + 30)
src.copy_area(0, 0, src.width / 2, src.height / 2, dst, 10, 20)
vbox.pack_start(Gtk::Image.new(dst))

dst = src.saturate_and_pixelate(0.3, true)
vbox.pack_start(Gtk::Image.new(dst))

dst = src.fill!(0xff000099) #RGBA
vbox.pack_start(Gtk::Image.new(dst))

w = Gtk::Window.new.add(vbox)
w.signal_connect('delete-event') do
  Gtk.main_quit
end

w.show_all

Gtk.main
