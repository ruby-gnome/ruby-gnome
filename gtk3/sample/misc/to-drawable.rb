#!/usr/bin/env ruby
=begin
  to_drawable.rb - GdkPixbuf::Pixbuf(GDK methods) sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

filename = "#{File.expand_path(File.dirname(__FILE__))}/gnome-logo-icon.png"

window = Gtk::Window.new("Drawable sample")
window.set_border_width(10)

pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)

drawing_area = Gtk::DrawingArea.new
window.add(drawing_area)

drawing_area.set_size_request(pixbuf.width + 40,
                              pixbuf.height + 40)
drawing_area.signal_connect("draw") do |_widget, cr|
  cr.set_source_rgb(1, 1, 1)
  cr.paint
  cr.set_source_pixbuf(pixbuf, 20, 20)
  cr.paint
  false
end

window.show_all
window.signal_connect("delete-event") { Gtk.main_quit }

Gtk.main
