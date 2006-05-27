#!/usr/bin/env ruby

require 'tempfile'
require "poppler"

if ARGV.size < 2
  puts "usage: #{$0} input.pdf output [scale_ratio] [rotate_angle]"
  exit(-1)
end

input, output, scale, rotate = ARGV
input_uri = "file://#{File.expand_path(input)}"
scale = (scale || 1.0).to_f
rotate = (rotate || 0).to_i % 360

ext_name = File.extname(output)[1..-1]
if ext_name
  output_type = ext_name.downcase
  output_type = "jpeg" if /jpg/ =~ output_type
else
  output_type = "png"
end

def compute_size(width, height, rotate)
  width = width.to_f
  height = height.to_f
  radius = 0
  unless rotate.zero?
    radius = rotate / 180.0 * Math::PI
    if (90 < rotate and rotate < 180) or
        (270 < rotate and rotate < 360)
      radius -= Math::PI / 2
    end
  end
  inner_angle1 = Math.atan(width / height)
  inner_angle2 = Math.atan(height / width)
  diagonal = Math.sqrt(width ** 2 + height ** 2)

  angle1 = radius + inner_angle1
  angle2 = radius + inner_angle2

  bottom1 = diagonal * Math.cos(angle1)
  length1 = (bottom1 * Math.tan(angle1)).abs.to_i
  bottom2 = diagonal * Math.cos(angle2)
  length2 = (bottom2 * Math.tan(angle2)).abs.to_i

  if (0 <= rotate and rotate <= 90) or
      (180 <= rotate and rotate <= 270)
    [length1, length2]
  else
    [length2, length1]
  end
end

def to_pixbuf_with_cairo(input, scale, rotate)
  doc = Poppler::Document.new(input)
  page = doc[0]
  width, height = page.size.collect {|x| x * scale}
  surface_width, surface_height = compute_size(width, height, rotate)

  surface = Cairo::ImageSurface.new(Cairo::FORMAT_ARGB32,
                                    surface_width, surface_height)
  cr = Cairo::Context.new(surface)

  half_width = surface_width / 2.0
  half_height = surface_height / 2.0
  cr.translate(half_width, half_height)
  cr.rotate(rotate / 180.0 * Math::PI)
  cr.translate(-half_width, -half_height)

  cr.translate((surface_width - width) / 2.0,
               (surface_height - height) / 2.0)
  cr.set_source_rgb(1, 1, 1)
  cr.rectangle(0, 0, width, height)
  cr.fill

  cr.scale(scale, scale)
  cr.render_poppler_page(page)
  temp = Tempfile.new("pdf2")
  cr.target.write_to_png(temp.path)
  cr.target.finish
  Gdk::Pixbuf.new(temp.path)
end

def to_pixbuf(input, scale, rotate)
  doc = Poppler::Document.new(input)
  page = doc[0]
  width, height = page.size.collect {|x| x * scale}
  pixbuf_width, pixbuf_height = compute_size(width, height, rotate)
  pixbuf = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8,
                           pixbuf_width, pixbuf_height)
  page.render(0, 0, width, height, scale, rotate, pixbuf)
  pixbuf
end

if Poppler.cairo_available?
  puts "using cairo..."
  pixbuf = to_pixbuf_with_cairo(input_uri, scale, rotate)
else
  pixbuf = to_pixbuf(input_uri, scale, rotate)
end

if pixbuf.nil?
  puts "Is it a PDF file?: #{input}"
  exit(-1)
end
puts "saving to #{output}(#{pixbuf.width}x#{pixbuf.height})..."
pixbuf.save(output, output_type)
