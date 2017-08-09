#!/usr/bin/env ruby

require "tempfile"
require "optparse"
require "poppler"
require "gdk_pixbuf2"

page = 0
scale = 1.0
rotate = 0

parser = OptionParser.new
parser.banner = <<-BANNER
Usage: #{parser.program_name} INPUT.PDF OUTPUT [options]

#{parser.program_name} coverts the target page in INPUT.PDF to an image file.
Many popular image formats such as PNG and JPEG are supported.
You can specify output image format by OUTPUT file extension.
For example, the following comand generates PNG file:

  % #{$0} INPUT.PDF OUTPUT.png

BANNER

parser.on("--page=PAGE", Integer,
          "Use PAGE as the target page",
          "(0-origin)",
          "[#{page}]") do |value|
  page = value.to_i
end

parser.on("--scale=RATIO", Float,
          "Scale the target page in INPUT.PDF RATIO times larger in OUTPUT",
          "[#{scale}]") do |value|
  scale = value.to_f
end

parser.on("--rotate=DEGREE", Integer,
          "Rotate the target page in INPUT.PDF DEGREE degrees clockwise in OUTPUT",
          "(available values: 0...360)",
          "[#{rotate}]") do |value|
  rotate = value.to_i % 360
end

rest_args = parser.parse!
if rest_args.size != 2
  puts(parser.help)
  exit(false)
end
input, output = rest_args

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

def to_pixbuf_with_cairo(input, page_number, scale, rotate)
  doc = Poppler::Document.new(input)
  page = doc[page_number]
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
  GdkPixbuf::Pixbuf.new(:file => temp.path)
end

pixbuf = to_pixbuf_with_cairo(input, page, scale, rotate)

if pixbuf.nil?
  puts "Is it a PDF file?: #{input}"
  exit(-1)
end
puts "saving to #{output}(#{pixbuf.width}x#{pixbuf.height})..."
pixbuf.save(output, output_type)
