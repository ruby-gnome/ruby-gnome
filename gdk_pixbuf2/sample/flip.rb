#!/usr/bin/env ruby
=begin
  flip.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2005,2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: flip.rb,v 1.3 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk2'


filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

if str = Gtk.check_version(2, 6, 0)
  puts "This sample requires GTK+ 2.6.0 or later"
  puts str
  exit
end

vbox = Gtk::VBox.new

src =  Gdk::Pixbuf.new(filename)
vbox.add(Gtk::Image.new(src))

# Horizontal
dst = src.flip(true)
vbox.add(Gtk::Image.new(dst))

# Vertical
dst2 = src.flip(false)
vbox.add(Gtk::Image.new(dst2))

window = Gtk::Window.new
window.signal_connect('delete-event') do
  Gtk.main_quit
end

window.add(vbox).show_all

Gtk.main

