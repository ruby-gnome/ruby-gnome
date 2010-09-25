#!/usr/bin/env ruby
=begin
  pangorenderer.rb - Ruby/GTK sample script.

  Copyright (c) 2005,2006  Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: pangorenderer.rb,v 1.5 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

RADIUS = 80
N_WORDS = 16

if str = Gtk.check_version(2, 6, 0)
  puts "This sample requires GTK+ 2.6.0 or later"
  puts str
  exit
end

win = Gtk::Window.new("Gdk::PangoRenderer sample")
win.signal_connect("destroy"){Gtk.main_quit}
win.set_default_size(400, 400)
win.realize

matrix = Pango::Matrix.new

renderer = Gdk::PangoRenderer.default
renderer.drawable = win.window
renderer.gc = Gdk::GC.new(win.window)
width, height = win.size

device_radius = [width, height].min / 6.0

matrix.translate!(device_radius + (width - 2 * device_radius) / 2.0,
		  device_radius + (height - 2 * device_radius) / 2.0)
matrix.scale!(device_radius / RADIUS, device_radius / RADIUS)

context = Gdk::Pango.context
layout = Pango::Layout.new(context)
layout.text = "Ruby-GNOME2"
layout.font_description = Pango::FontDescription.new("Sans Bold 14")

background = Gdk::Color.new(65535, 65535, 65535)
Gdk::Colormap.system.alloc_color(background, false, true)
win.window.background = background

win.signal_connect("expose_event") do
  (0...N_WORDS).each do |i|
    rotated_matrix = matrix.dup
    angle = 360 * i / N_WORDS.to_f
    color = Gdk::Color.new(65535 * rand, 65535 * rand, 65535 * rand)
    renderer.set_override_color(Pango::Renderer::PART_FOREGROUND, color)
    
    rotated_matrix.rotate!(angle)
    context.set_matrix(rotated_matrix)
    layout.context_changed
    width, height = layout.size
    renderer.draw_layout(layout, width / 2, RADIUS * Pango::SCALE)
  end
end

win.show_all

Gtk.main
