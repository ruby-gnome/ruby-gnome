#!/usr/bin/env ruby
=begin
  rotate.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2005-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.

  $Id: rotate.rb,v 1.3 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk3'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

vbox = Gtk::Box.new(:vertical)

src =  GdkPixbuf::Pixbuf.new(file: filename)
vbox.add(Gtk::Image.new(pixbuf: src))

dst = src.rotate(:counterclockwise)
vbox.add(Gtk::Image.new(pixbuf: dst))

dst2 = src.rotate(:upsidedown)
vbox.add(Gtk::Image.new(pixbuf: dst2))

window = Gtk::Window.new
window.signal_connect('delete-event') do
  Gtk.main_quit
end

window.add(vbox).show_all

Gtk.main
