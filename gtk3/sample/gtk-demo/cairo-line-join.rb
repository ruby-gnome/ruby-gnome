# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-line-join.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Line Join

This demo shows how to use GDK and cairo to set line join.
From http://cairographics.org/samples/set_line_join.html.
=end
require 'common'

module Demo
  class CairoLineJoin < CairoWindow
    def initialize
      super('cairo line join')
    end

    def draw(cr)
      cr.set_line_width(0.16)

      cr.move_to(0.3, 0.33)
      cr.rel_line_to(0.2, -0.2)
      cr.rel_line_to(0.2, 0.2)
      cr.set_line_join(Cairo::LINE_JOIN_MITER) # default
      cr.stroke

      cr.move_to(0.3, 0.63)
      cr.rel_line_to(0.2, -0.2)
      cr.rel_line_to(0.2, 0.2)
      cr.set_line_join(Cairo::LINE_JOIN_BEVEL)
      cr.stroke

      cr.move_to(0.3, 0.93)
      cr.rel_line_to(0.2, -0.2)
      cr.rel_line_to(0.2, 0.2)
      cr.set_line_join(Cairo::LINE_JOIN_ROUND)
      cr.stroke
    end
  end
end
