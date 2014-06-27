# Copyright (c) 2005-2014  Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: rotated_text.rb,v 1.2 2005/03/05 15:01:16 mutoh Exp $
=begin
= Rotated Text

This demo shows how to use Cairo and Pango to draw rotated and transformed text.
=end
require "common"

module Demo
  class RotatedText < BasicWindow
    RADIUS = 150
    N_WORDS = 10
    FONT = "Sans Bold 27"

    def initialize
      super("Rotated Text")

      drawing_area = Gtk::DrawingArea.new
      add(drawing_area)
      drawing_area.override_background_color(:normal, Gdk::RGBA.new(1.0,
                                                                    1.0,
                                                                    1.0,
                                                                    0.0))

      drawing_area.signal_connect("draw") do |_drawing_area, cairo_context|
        cairo_context.set_source_rgb(1.0, 1.0, 1.0)
        cairo_context.paint
        cairo_context.save do
          width, height = size
          cairo_context.scale(width.to_f / (RADIUS * 2),
                              height.to_f / (RADIUS * 2))
        draw_text(cairo_context)
        end
      end
    end

    private
    def draw_text(cairo_context)
      # Center coordinates on the middle of the region we are drawing
      cairo_context.translate(RADIUS, RADIUS)

      # Create a PangoLayout, set the font and text
      layout = cairo_context.create_pango_layout

      layout.text = "Text"
      layout.font_description = Pango::FontDescription.new(FONT)

      # Draw the layout N_WORDS times in a circle
      N_WORDS.times do |i|
        angle = (360.0 * i) / N_WORDS

        cairo_context.save do
          # Gradient from red at angle == 60 to blue at angle == 240
          red = (1 + Math.cos((angle - 60) * Math::PI / 180.0)) / 2
          cairo_context.set_source_rgb(red, 0.0, 1.0 - red)

          cairo_context.rotate(angle * Math::PI / 180.0)

          # Inform Pango to re-layout the text with the new transformation
          cairo_context.update_pango_layout(layout)

          width, height = layout.size
          cairo_context.move_to(-(width / Pango::SCALE) / 2, -RADIUS)
          cairo_context.show_pango_layout(layout)
        end
      end
    end
  end
end
