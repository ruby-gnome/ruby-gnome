# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-text.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Text

This demo shows how to use GDK and cairo to show text.
From http://cairographics.org/samples/text.html.
=end
require 'common'

module Demo
  class CairoText < CairoWindow
    def initialize
      super('cairo text')
    end

    def draw(cr)
      cr.select_font_face("Sans",
                          Cairo::FONT_SLANT_NORMAL,
                          Cairo::FONT_WEIGHT_BOLD)
      cr.set_font_size(0.35)

      cr.move_to(0.04, 0.53)
      cr.show_text("Hello")

      cr.move_to(0.27, 0.65)
      cr.text_path("void")
      cr.set_source_rgba(0.5, 0.5, 1)
      cr.fill_preserve
      cr.set_source_rgba(0, 0, 0)
      cr.set_line_width(0.01)
      cr.stroke

      # draw helping lines
      cr.set_source_rgba(1,0.2,0.2, 0.6)
      cr.arc(0.04, 0.53, 0.02, 0, 2 * Math::PI)
      cr.arc(0.27, 0.65, 0.02, 0, 2 * Math::PI)
      cr.fill
    end
  end
end
