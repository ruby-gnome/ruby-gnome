#!/usr/bin/env ruby
=begin
  rotate.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2005-2016 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: rotate.rb,v 1.3 2006/06/17 14:38:08 mutoh Exp $
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

src =  GdkPixbuf::Pixbuf.new(:file => filename)
vbox.add(Gtk::Image.new(src))

dst = src.rotate(:counterclockwise)
vbox.add(Gtk::Image.new(dst))

dst2 = src.rotate(:upsidedown)
vbox.add(Gtk::Image.new(dst2))

window = Gtk::Window.new
window.signal_connect('delete-event') do
  Gtk.main_quit
end

window.add(vbox).show_all

Gtk.main

