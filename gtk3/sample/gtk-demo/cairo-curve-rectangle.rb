# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-curve-rectangle.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Curve Rectangle

This demo shows how to use GDK and cairo to draw curve rectangle.
From http://cairographics.org/samples/curve_rectangle.html.
=end
require 'common'

module Demo
  class CairoCurveRectangle < CairoWindow
    # a custom shape, that could be wrapped in a function
    X0 = 0.1 #< parameters like Cairo::Context#rectangle
    Y0 = 0.1
    RECT_WIDTH = 0.8
    RECT_HEIGHT = 0.8
    RADIUS = 0.4 #< and an approximate curvature radius

    X1 = X0 + RECT_WIDTH
    Y1 = Y0 + RECT_HEIGHT

    def initialize
      super('cairo curve rectangle')
    end

    def draw(cr)
      return if (RECT_WIDTH <= 0 || RECT_HEIGHT <= 0)

      if (RECT_WIDTH / 2 < RADIUS)
        if (RECT_HEIGHT / 2 < RADIUS)
          cr.move_to(X0, (Y0 + Y1) / 2)
          cr.curve_to(X0 ,Y0, X0, Y0, (X0 + X1) / 2, Y0)
          cr.curve_to(X1, Y0, X1, Y0, X1, (Y0 + Y1) / 2)
          cr.curve_to(X1, Y1, X1, Y1, (X1 + X0) / 2, Y1)
          cr.curve_to(X0, Y1, X0, Y1, X0, (Y0 + Y1) / 2)
        else
          cr.move_to(X0, Y0 + RADIUS)
          cr.curve_to(X0 ,Y0, X0, Y0, (X0 + X1) / 2, Y0)
          cr.curve_to(X1, Y0, X1, Y0, X1, Y0 + RADIUS)
          cr.line_to(X1 , Y1 - RADIUS)
          cr.curve_to(X1, Y1, X1, Y1, (X1 + X0) / 2, Y1)
          cr.curve_to(X0, Y1, X0, Y1, X0, Y1- RADIUS)
        end
      else
        if (RECT_HEIGHT / 2 < RADIUS)
          cr.move_to(X0, (Y0 + Y1) / 2)
          cr.curve_to(X0, Y0, X0 , Y0, X0 + RADIUS, Y0)
          cr.line_to(X1 - RADIUS, Y0)
          cr.curve_to(X1, Y0, X1, Y0, X1, (Y0 + Y1) / 2)
          cr.curve_to(X1, Y1, X1, Y1, X1 - RADIUS, Y1)
          cr.line_to(X0 + RADIUS, Y1)
          cr.curve_to(X0, Y1, X0, Y1, X0, (Y0 + Y1) / 2)
        else
          cr.move_to(X0, Y0 + RADIUS)
          cr.curve_to(X0 , Y0, X0 , Y0, X0 + RADIUS, Y0)
          cr.line_to(X1 - RADIUS, Y0)
          cr.curve_to(X1, Y0, X1, Y0, X1, Y0 + RADIUS)
          cr.line_to(X1 , Y1 - RADIUS)
          cr.curve_to(X1, Y1, X1, Y1, X1 - RADIUS, Y1)
          cr.line_to(X0 + RADIUS, Y1)
          cr.curve_to(X0, Y1, X0, Y1, X0, Y1- RADIUS)
        end
      end

      cr.close_path

      cr.set_source_rgba(0.5,0.5,1)
      cr.fill_preserve
      cr.set_source_rgba(0.5, 0, 0, 0.5)
      cr.stroke
    end
  end
end
