#!/usr/bin/env ruby

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

pixbuf = RSVG.pixbuf_from_file_at_zoom(input, ratio, ratio)
if pixbuf.nil?
  puts "Is it a SVG file?: #{input}"
  exit(-1)
end
puts "saving to #{output}(#{pixbuf.width}x#{pixbuf.height})..."
pixbuf.save(output, output_type)
