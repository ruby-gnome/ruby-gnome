# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-text-align-center.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Text Align Center

This demo shows how to use GDK and cairo to show text in center.
From http://cairographics.org/samples/text_align_center.html.
=end
require 'common'

module Demo
  class CairoTextAlignCenter < CairoWindow
    TEXT = "Ruby"

    def initialize
      super('cairo text align center')
    end

    def draw(cr)
      cr.select_font_face("Sans",
                          Cairo::FONT_SLANT_NORMAL,
                          Cairo::FONT_WEIGHT_NORMAL)

      cr.set_font_size(0.2)
      extents = cr.text_extents(TEXT)
      x = 0.5 - (extents.width / 2 + extents.x_bearing)
      y = 0.5 - (extents.height / 2 + extents.y_bearing)

      cr.move_to(x, y)
      cr.show_text(TEXT)

      # draw helping lines
      cr.set_source_rgba(1, 0.2, 0.2, 0.6)
      cr.arc(x, y, 0.05, 0, 2 * Math::PI)
      cr.fill
      cr.move_to(0.5, 0)
      cr.rel_line_to(0, 1)
      cr.move_to(0, 0.5)
      cr.rel_line_to(1, 0)
      cr.stroke
    end
  end
end
