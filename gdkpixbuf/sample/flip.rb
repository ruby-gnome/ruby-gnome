#!/usr/bin/env ruby
=begin
  flip.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: flip.rb,v 1.2 2005/03/22 17:41:24 silicio Exp $
=end

require 'gtk2'


filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

Gtk.init

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

