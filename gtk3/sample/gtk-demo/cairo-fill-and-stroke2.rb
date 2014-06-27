# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-fill-and-stroke2.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Fill And Stroke2

This demo shows how to use GDK and cairo to fill and stroke.
From http://cairographics.org/samples/fill_and_stroke2.html.
=end
require 'common'

module Demo
  class CairoFillAndStroke2 < CairoWindow
    def initialize
      super('cairo fill and stroke2')
    end

    def draw(cr)
      cr.move_to(0.5, 0.1)
      cr.line_to(0.9, 0.9)
      cr.rel_line_to(-0.4, 0.0)
      cr.curve_to(0.2, 0.9, 0.2, 0.5, 0.5, 0.5)
      cr.close_path

      cr.move_to(0.25, 0.1)
      cr.rel_line_to(0.2, 0.2)
      cr.rel_line_to(-0.2, 0.2)
      cr.rel_line_to(-0.2, -0.2)
      cr.close_path

      cr.set_source_rgba(0, 0, 1)
      cr.fill_preserve
      cr.set_source_rgba(0, 0, 0)
      cr.stroke
    end
  end
end
