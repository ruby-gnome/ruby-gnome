#!/usr/bin/env ruby

require "poppler"

if ARGV.size != 2
  puts "usage: #{$0} input.pdf output.pdf"
  exit(-1)
end

input, output = ARGV

doc = Poppler::Document.new(input)

width, height = doc[0].size
Cairo::PDFSurface.new(output, width / 2, height) do |surface|
  context = Cairo::Context.new(surface)

  doc.each do |page|
    width, height = page.size
    half_width = width / 2
    context.save do
      context.rectangle(0, 0, half_width, height)
      context.clip
      page.render(context)
      context.show_page
    end

    context.save do
      context.translate(-half_width, 0)
      context.rectangle(half_width, 0, half_width, height)
      context.clip
      page.render(context)
      context.show_page
    end
  end
end
