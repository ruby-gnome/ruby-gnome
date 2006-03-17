#!/usr/bin/env ruby

require 'tempfile'
require "rsvg2"

if ARGV.size < 2
  puts "usage: #{$0} input.svg output [scale_ratio]"
  exit(-1)
end

input, output, ratio = ARGV
ratio ||= 1.0
ratio = ratio.to_f

output_type = File.extname(output)[1..-1].downcase
output_type = "jpeg" if /jpg/ =~ output_type

def to_pixbuf_with_cairo(input, ratio)
  handle = RSVG::Handle.new_from_file(input)
  dim = handle.dimensions
  width = dim.width * ratio
  height = dim.height * ratio
  surface = Cairo::ImageSurface.new(Cairo::FORMAT_ARGB32, width, height)
  cr = Cairo::Context.new(surface)
  cr.scale(ratio, ratio)
  cr.render_rsvg(handle)
  temp = Tempfile.new("svg2")
  cr.target.write_to_png(temp.path)
  cr.target.finish
  Gdk::Pixbuf.new(temp.path)
end

def to_pixbuf(input, ratio)
  RSVG.pixbuf_from_file_at_zoom(input, ratio, ratio)
end

if RSVG.cairo_available?
  puts "using cairo..."
  pixbuf = to_pixbuf_with_cairo(input, ratio)
else
  pixbuf = to_pixbuf(input, ratio)
end

if pixbuf.nil?
  puts "Is it a SVG file?: #{input}"
  exit(-1)
end
puts "saving to #{output}(#{pixbuf.width}x#{pixbuf.height})..."
pixbuf.save(output, output_type)
