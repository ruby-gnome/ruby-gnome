#!/usr/local/bin/ruby
=begin
  to_drawable.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: to_drawable.rb,v 1.3 2002/11/05 11:20:46 mutoh Exp $
=end

require 'gtk2'
require 'gdk_pixbuf2'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

Gtk.init

pixbuf =  Gdk::Pixbuf.new(filename)

w = Gtk::Window.new
w.signal_connect('delete-event') do
  Gtk.main_quit
end

d = Gtk::DrawingArea.new
d.signal_connect('expose-event') do |w, e|
  unless e.count > 0
    pixbuf.scale(w.allocation.width, w.allocation.height).
      render_to_drawable(w.window, w.style.fg_gc(Gtk::STATE_NORMAL), 0, 0,
			 0, 0, w.allocation.width, w.allocation.height)
  end
  GC.start
  true
end
d.show
w.add d
w.show

Gtk.main
