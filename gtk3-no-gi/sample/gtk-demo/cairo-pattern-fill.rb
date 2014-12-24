# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-pattern-fill.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Pattern Fill

This demo shows how to use GDK and cairo to fill by pattern.
From http://cairographics.org/samples/pattern_fill.html.
=end
require 'common'

module Demo
  class CairoPatternFill < CairoWindow
    X_FUZZ = 0.08
    Y_FUZZ = 0.08

    X_INNER_RADIUS = 0.3
    Y_INNER_RADIUS = 0.2

    X_OUTER_RADIUS = 0.45
    Y_OUTER_RADIUS = 0.35

    SPIKES = 10

    TEXT = "Ruby!"

    def initialize
      super('cairo pattern fill')
    end

    def draw(cr)
      srand(45)

      cr.set_line_width(0.01)

      x = y = nil
      0.step(SPIKES * 2 - 1, 2) do |i|
        x = x_position(i, X_INNER_RADIUS)
        y = y_position(i, Y_INNER_RADIUS)

        if (i == 0)
          cr.move_to(x, y)
        else
          cr.line_to(x, y)
        end

        i += 1

        x = x_position(i, X_OUTER_RADIUS)
        y = y_position(i, Y_OUTER_RADIUS)

        cr.line_to(x, y)
      end

      cr.close_path
      cr.stroke

      cr.select_font_face("Sans",
                          Cairo::FONT_SLANT_NORMAL,
                          Cairo::FONT_WEIGHT_BOLD)

      cr.move_to(x - 0.5, y)
      cr.set_font_size(0.1)
      cr.text_path(TEXT)
      cr.set_source_rgba(1, 0, 0.5)
      cr.fill

      cr.set_font_size(0.2)
      extents = cr.text_extents(TEXT)
      x = 0.5 - (extents.width / 2 + extents.x_bearing)
      y = 0.5 - (extents.height / 2 + extents.y_bearing)

      cr.move_to(x, y)
      cr.text_path(TEXT)
      cr.set_source_rgba(1, 0, 0)
      cr.stroke
    end

    def x_position(i, radius)
      0.5 + Math.cos(Math::PI * i / SPIKES) * radius + rand * X_FUZZ
    end

    def y_position(i, radius)
      0.5 + Math.sin(Math::PI * i / SPIKES) * radius + rand * Y_FUZZ
    end
  end
end
