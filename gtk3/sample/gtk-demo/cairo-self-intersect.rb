# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-self-intersect.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Self Intersect

This demo shows how to use GDK and cairo to show cross.
From http://cairographics.org/samples/xxx_self_intersect.html.
=end
require 'common'

module Demo
  class CairoSelfIntersect < CairoWindow
    def initialize
      super('cairo self intersect')
    end

    def draw(cr)
      cr.move_to(0.3, 0.3)
      cr.line_to(0.7, 0.3)

      cr.line_to(0.5, 0.3)
      cr.line_to(0.5, 0.7)

      cr.set_line_width(0.22)
      cr.set_line_cap(Cairo::LINE_CAP_ROUND)
      cr.set_line_join(Cairo::LINE_JOIN_ROUND)
      cr.stroke
    end
  end
end
