#!/usr/bin/env ruby
=begin
  rgtk+cairo.rb - Ruby/GTK2 using Ruby/Cairo sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  Heavily inspired from png.rb example of rcairo.
=end

require "gtk3"

w = Gtk::Window.new
w.add(vb = Gtk::Box.new(:vertical))
vb.add(da = Gtk::DrawingArea.new)

da.set_size_request(200, 200)

da.signal_connect "draw"  do |_widget, cr|
  # fill background with white
  cr.set_source_rgba(1.0, 1.0, 1.0, 1.0)
  cr.paint

  # create shape
  cr.move_to(50, 50)
  cr.curve_to(100, 25, 100, 75, 150, 50)
  cr.line_to(150, 150)
  cr.line_to(50, 150)
  cr.close_path

  cr.set_source_rgb(0.0, 0.0, 0.0)
  cr.fill_preserve
  cr.set_source_rgb(1.0, 0.0, 0.0)
  cr.set_line_join(Cairo::LINE_JOIN_MITER)
  cr.set_line_width(4)
  cr.stroke
end

w.signal_connect("destroy") { Gtk.main_quit }
w.show_all
Gtk.main
