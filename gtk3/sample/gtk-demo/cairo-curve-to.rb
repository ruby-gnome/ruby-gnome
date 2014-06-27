# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-curve-to.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Curve To

This demo shows how to use GDK and cairo to draw curve.
From http://cairographics.org/samples/curve_to.html.
=end
require 'common'

module Demo
  class CairoCurveTo < CairoWindow
    X = 0.1
    Y = 0.5
    X1 = 0.4
    Y1 = 0.9
    X2 = 0.6
    Y2 = 0.1
    X3 = 0.9
    Y3 = 0.5

    def initialize
      super('cairo curve to')
    end

    def draw(cr)
      cr.move_to(X, Y)
      cr.curve_to(X1, Y1, X2, Y2, X3, Y3)

      cr.stroke

      cr.set_source_rgba(1, 0.2, 0.2, 0.6)
      cr.set_line_width(0.03)
      cr.move_to(X, Y)
      cr.line_to(X1, Y1)
      cr.move_to(X2, Y2)
      cr.line_to(X3, Y3)
      cr.stroke
    end
  end
end
