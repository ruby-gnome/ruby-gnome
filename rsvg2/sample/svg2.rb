#!/usr/bin/env ruby
=begin
  svg2.rb - Ruby/RSVG sample script.

  Copyright (c) 2006-2017 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

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

def to_pixbuf(input, ratio)
  handle = nil
  Dir.chdir(File.dirname(File.expand_path(input))) do
    handle = Rsvg::Handle.new(:path => input)
  end
  dim = handle.dimensions
  width = dim.width * ratio
  height = dim.height * ratio
  surface = Cairo::ImageSurface.new(Cairo::FORMAT_ARGB32, width, height)
  cr = Cairo::Context.new(surface)
  cr.scale(ratio, ratio)
  cr.render_rsvg_handle(handle)
  temp = Tempfile.new("svg2")
  cr.target.write_to_png(temp.path)
  cr.target.finish
  GdkPixbuf::Pixbuf.new(:file => temp.path)
end

pixbuf = to_pixbuf(input, ratio)
if pixbuf.nil?
  puts "Is it a SVG file?: #{input}"
  exit(-1)
end
puts "saving to #{output}(#{pixbuf.width}x#{pixbuf.height})..."
pixbuf.save(output, output_type)
