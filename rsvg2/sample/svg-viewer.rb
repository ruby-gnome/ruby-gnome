#!/usr/bin/env ruby

require 'gtk2'
require 'rsvg2'

if ARGV.size != 1
  puts "usage: #{$0} input.svg"
  exit(-1)
end

input = ARGV.shift

handle = Rsvg::Handle.new(:path => input)
width, height = handle.dimensions.to_a

window = Gtk::Window.new
window.set_default_size(width, height)
area = Gtk::DrawingArea.new

window.signal_connect(:destroy) do
  Gtk.main_quit
end

area.signal_connect(:expose_event) do |widget, event|
  context = widget.window.create_cairo_context
  window_width, window_height = widget.window.size
  context.scale(window_width.to_f / width, window_height.to_f / height)
  context.render_rsvg_handle(handle)
end

window.add(area)
window.show_all

Gtk.main
