#!/usr/bin/env ruby

require 'poppler'

input = ARGV.first
output = input.sub(/\.pdf$/, "-numbered.pdf")

doc = Poppler::Document.new(input)
first_page = doc[0]

Cairo::PDFSurface.new(output, *first_page.size) do |surface|
  context = Cairo::Context.new(surface)

  doc.each_with_index do |page, i|
    width, height = page.size
    surface.set_size(width, height)
    context.save do
      context.render_poppler_page(page)
    end
    context.save do
      layout = context.create_pango_layout
      layout.text = i.to_s
      layout.width = width * Pango::SCALE
      layout.alignment = Pango::Layout::ALIGN_CENTER
      layout.font_description = Pango::FontDescription.new("Sans 288")
      context.move_to(0, (height - layout.pixel_size[1]) / 2.0)
      context.set_source_rgba(0.1, 0.1, 0.1, 0.5)
      context.show_pango_layout(layout)
    end
    context.show_page
  end
end
