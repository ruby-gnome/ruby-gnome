# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-text-extents.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Text Extents

This demo shows how to use GDK and cairo to show text extents.
From http://cairographics.org/samples/text.html.
=end
require 'common'

module Demo
  class CairoTextExtents < CairoWindow
    TEXT = "Ruby"

    def initialize
      super('cairo text')
    end

    def draw(cr)
      cr.select_font_face("Sans",
                              Cairo::FONT_SLANT_NORMAL,
                              Cairo::FONT_WEIGHT_NORMAL)

      cr.set_font_size(0.3)
      extents = cr.text_extents(TEXT)

      x = 0.1
      y = 0.6

      cr.move_to(x, y)
      cr.show_text(TEXT)

      # draw helping lines
      cr.set_source_rgba(1, 0.2, 0.2, 0.6)
      cr.arc(x, y, 0.05, 0, 2 * Math::PI)
      cr.fill
      cr.move_to(x, y)
      cr.rel_line_to(0, -extents.height)
      cr.rel_line_to(extents.width, 0)
      cr.rel_line_to(extents.x_bearing, -extents.y_bearing)
      cr.stroke
    end
  end
end
