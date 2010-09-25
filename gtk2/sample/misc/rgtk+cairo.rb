#!/usr/bin/env ruby
=begin
  rgtk+cairo.rb - Ruby/GTK2 using Ruby/Cairo sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  Heavily inspired from png.rb example of rcairo.

  $Id: rgtk+cairo.rb,v 1.3 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'
require 'cairo'

w = Gtk::Window.new.add(vb = Gtk::VBox.new)
vb.add(da = Gtk::DrawingArea.new)
da.set_size_request(200, 200)

da.signal_connect('expose-event') { |widget, event|
   cr = widget.window.create_cairo_context

   # fill background with white
   cr.set_source_rgba(1.0, 1.0, 1.0)
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
}

w.signal_connect("destroy"){Gtk.main_quit}
w.show_all
Gtk.main
