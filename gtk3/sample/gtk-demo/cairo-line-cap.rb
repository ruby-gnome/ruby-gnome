# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-line-cap.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Line Cap

This demo shows how to use GDK and cairo to set line cap.
From http://cairographics.org/samples/set_line_cap.html.
=end
require 'common'

module Demo
  class CairoLineCap < CairoWindow
    def initialize
      super('cairo line cap')
    end

    def draw(cr)
      cr.set_line_width(0.12)

      cr.set_line_cap(Cairo::LINE_CAP_BUTT) # default
      cr.move_to(0.25, 0.2)
      cr.line_to(0.25, 0.8)
      cr.stroke

      cr.set_line_cap(Cairo::LINE_CAP_ROUND)
      cr.move_to(0.5, 0.2)
      cr.line_to(0.5, 0.8)
      cr.stroke

      cr.set_line_cap(Cairo::LINE_CAP_SQUARE)
      cr.move_to(0.75, 0.2)
      cr.line_to(0.75, 0.8)
      cr.stroke

      # draw helping lines
      cr.set_source_rgba(1, 0.2, 0.2)
      cr.set_line_width(0.01)

      cr.move_to(0.25, 0.2)
      cr.line_to(0.25, 0.8)

      cr.move_to(0.5, 0.2)
      cr.line_to(0.5, 0.8)

      cr.move_to(0.75, 0.2)
      cr.line_to(0.75, 0.8)

      cr.stroke
    end
  end
end
