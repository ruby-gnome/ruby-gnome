# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-arc.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Arc

This demo shows how to use GDK and cairo to draw arc.
From http://cairographics.org/samples/arc.html.
=end
require 'common'

module Demo
  class CairoArc < CairoWindow
    CENTER_X = 0.5
    CENTER_Y = 0.5
    RADIUS = 0.4
    ANGLE1 = 45.0 * (Math::PI / 180.0)  # angles are specified
    ANGLE2 = 180.0 * (Math::PI / 180.0) # in radians

    def initialize
      super('cairo arc')
    end

    def draw(cr)
      cr.arc(CENTER_X, CENTER_Y, RADIUS, ANGLE1, ANGLE2)
      cr.stroke

      # draw helping lines
      cr.set_source_rgba(1, 0.2, 0.2, 0.6)
      cr.arc(CENTER_X, CENTER_Y, 0.05, 0, 2 * Math::PI)
      cr.fill
      cr.set_line_width(0.03)
      cr.arc(CENTER_X, CENTER_Y, RADIUS, ANGLE1, ANGLE1)
      cr.line_to(CENTER_X, CENTER_Y)
      cr.arc(CENTER_X, CENTER_Y, RADIUS, ANGLE2, ANGLE2)
      cr.line_to(CENTER_X, CENTER_Y)
      cr.stroke
    end
  end
end
