#!/usr/bin/env ruby

require "optparse"
require "poppler"

x_margin_ratio = 0.1
y_margin_ratio = 0.1
crop_x_offset = nil
crop_y_offset = nil
crop_width = nil
crop_height = nil
parser = OptionParser.new
parser.banner += "input.pdf output.pdf"
parser.on("--x-margin-ratio=RATIO",
          Float,
          "Margin ratio for X",
          "(#{x_margin_ratio})") do |ratio|
  x_margin_ratio = ratio
end
parser.on("--y-margin-ratio=RATIO",
          Float,
          "Margin ratio for Y",
          "(#{y_margin_ratio})") do |ratio|
  y_margin_ratio = ratio
end
parser.on("--crop-x-offset=OFFSET",
          Float,
          "X offset for crop",
          "(0)") do |offset|
  crop_x_offset = offset
end
parser.on("--crop-y-offset=OFFSET",
          Float,
          "Y offset for crop",
          "(0)") do |offset|
  crop_y_offset = offset
end
parser.on("--crop-width=WIDTH",
          Float,
          "Width for crop",
          "(Original width)") do |width|
  crop_width = width
end
parser.on("--crop-height=HEIGHT",
          Integer,
          "Height for crop",
          "(Original height)") do |height|
  crop_height = height
end
parser.parse!
if ARGV.size != 2
  puts(parser.to_s)
  exit(false)
end

input, output = ARGV

doc = Poppler::Document.new(input)

width, height = doc[0].size
if crop_x_offset or crop_y_offset or crop_width or crop_height
  x_offset = (crop_x_offset || 0)
  y_offset = (crop_y_offset || 0)
  cropped_width = crop_width || width
  cropped_height = crop_height || height
  x_margin = cropped_width * x_margin_ratio
  y_margin = cropped_height * y_margin_ratio
  cropped_width += x_margin * 2
  cropped_height += y_margin * 2
else
  x_offset = 0
  y_offset = 0
  x_margin = width * x_margin_ratio
  y_margin = height * y_margin_ratio
  cropped_width = width - (2 * x_margin)
  cropped_height = height - (2 * y_margin)
end
Cairo::PDFSurface.new(output, cropped_width, cropped_height) do |surface|
  context = Cairo::Context.new(surface)

  doc.each do |page|
    width, height = page.size
    context.save do
      context.translate(-(x_margin + x_offset),
                        -(y_margin + y_offset))
      context.rectangle(x_margin + x_offset,
                        y_margin + y_offset,
                        cropped_width,
                        cropped_height)
      context.clip
      page.render(context)
      context.show_page
    end
  end
end
