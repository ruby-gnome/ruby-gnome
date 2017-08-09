#!/usr/bin/env ruby

require "poppler"

if ARGV.size < 1
  puts "usage: #{$0} input.pdf"
  exit(-1)
end

input, = ARGV

output = input.sub(/\..+$/, ".svg")
output = "#{output}.svg" if input == output

doc = Poppler::Document.new(input)

width, height = doc[0].size
Cairo::SVGSurface.new(output, width, height) do |surface|
  surface.restrict_to_version("1_2")

  context = Cairo::Context.new(surface)
  doc.each do |page|
    page.render(context)
    context.show_page
  end
end
