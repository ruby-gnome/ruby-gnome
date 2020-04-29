#!/usr/bin/env ruby

require 'gtk3'
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
window.signal_connect(:destroy) { Gtk.main_quit }

area = Gtk::DrawingArea.new
area.signal_connect(:draw) do |widget, _event|
  context = widget.window.create_cairo_context
  window_width, window_height = widget.window.width, widget.window.height
  context.scale(window_width.to_f / width, window_height.to_f / height)
  context.render_rsvg_handle(handle)
end

window.add(area)
window.show_all

Gtk.main
