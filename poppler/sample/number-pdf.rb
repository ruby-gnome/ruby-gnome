#!/usr/bin/env ruby
# Copyright (C) 2017  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

# usage :
#   ruby number-pdf.rb example.pdf

require "poppler"
require "pango"

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
      layout.alignment = :center
      layout.font_description = Pango::FontDescription.new("Sans 288")
      context.move_to(0, (height - layout.pixel_size[1]) / 2.0)
      context.set_source_rgba(0.1, 0.1, 0.1, 0.5)
      context.show_pango_layout(layout)
    end
    context.show_page
  end
end
