#!/usr/local/bin/ruby
=begin
  gc.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: gc.rb,v 1.3 2003/02/01 16:46:22 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.set_default_size(300, 300)
window.set_app_paintable(true)
window.realize

drawable = window.window      
gc = Gdk::GC.new(drawable)            

red   = Gdk::Color.new(65535, 0, 0)
green = Gdk::Color.new(0, 65535, 0)
colormap = Gdk::Colormap.system
colormap.alloc_color(red,   false, true)
colormap.alloc_color(green, false, true)

window.signal_connect("expose-event") do |win, evt|
  gc.set_foreground(red)
  gc.set_background(green)
  
  gc.set_line_attributes(5, Gdk::GC::LINE_SOLID, Gdk::GC::CAP_NOT_LAST, Gdk::GC::JOIN_MITER)
  drawable.draw_line(gc, 20, 10, 150, 10)
  gc.set_line_attributes(5, Gdk::GC::LINE_ON_OFF_DASH, Gdk::GC::CAP_NOT_LAST, Gdk::GC::JOIN_MITER)
  drawable.draw_line(gc, 20, 30, 150, 30)
  gc.set_line_attributes(5, Gdk::GC::LINE_DOUBLE_DASH, Gdk::GC::CAP_NOT_LAST, Gdk::GC::JOIN_MITER)
  drawable.draw_line(gc, 20, 50, 150, 50)
  
  gc.set_line_attributes(30, Gdk::GC::LINE_SOLID, Gdk::GC::CAP_NOT_LAST, Gdk::GC::JOIN_MITER)
  drawable.draw_line(gc, 20, 100, 120, 100)
  gc.set_line_attributes(30, Gdk::GC::LINE_SOLID, Gdk::GC::CAP_BUTT, Gdk::GC::JOIN_MITER)
  drawable.draw_line(gc, 20, 140, 120, 140)
  gc.set_line_attributes(30, Gdk::GC::LINE_SOLID, Gdk::GC::CAP_ROUND, Gdk::GC::JOIN_MITER)
  drawable.draw_line(gc, 20, 180, 120, 180)
  gc.set_line_attributes(30, Gdk::GC::LINE_SOLID, Gdk::GC::CAP_PROJECTING, Gdk::GC::JOIN_MITER)
  drawable.draw_line(gc, 20, 220, 120, 220)
  
  gc.set_line_attributes(40, Gdk::GC::LINE_SOLID, Gdk::GC::CAP_NOT_LAST, Gdk::GC::JOIN_MITER)
  drawable.draw_lines(gc, [[200, 80], [220, 30], [280, 50]])
  gc.set_line_attributes(40, Gdk::GC::LINE_SOLID, Gdk::GC::CAP_NOT_LAST, Gdk::GC::JOIN_ROUND)
  drawable.draw_lines(gc, [[200, 170], [220, 120], [280, 140]])
  gc.set_line_attributes(40, Gdk::GC::LINE_SOLID, Gdk::GC::CAP_NOT_LAST, Gdk::GC::JOIN_BEVEL)
  drawable.draw_lines(gc, [[200, 260], [220, 210], [280, 230]])
end

window.show_all
Gtk.main


