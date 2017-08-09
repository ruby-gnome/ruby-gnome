#!/usr/bin/env ruby

require "poppler"

if ARGV.size != 2
  puts "usage: #{$0} input.pdf output.pdf"
  exit(-1)
end

input, output = ARGV

doc = Poppler::Document.new(input)

width, height = doc[0].size
x_margin = width * 0.1
y_margin = height * 0.1
croped_width = width - (2 * x_margin)
croped_height = height - (2 * y_margin)
Cairo::PDFSurface.new(output, croped_width, croped_height) do |surface|
  context = Cairo::Context.new(surface)

  doc.each do |page|
    width, height = page.size
    context.save do
      context.translate(-x_margin, -y_margin)
      context.rectangle(x_margin, y_margin, croped_width, croped_height)
      context.clip
      page.render(context)
      context.show_page
    end
  end
end
