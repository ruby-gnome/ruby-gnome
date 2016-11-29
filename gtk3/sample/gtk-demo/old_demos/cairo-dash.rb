# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-dash.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Dash

This demo shows how to use GDK and cairo to show dash.
From http://cairographics.org/samples/xxx_dash.html.
=end
require 'common'

module Demo
  class CairoDash < CairoWindow
    def initialize
      super('cairo dash')
    end

    def draw(cr)
      dashes = [
        0.20,  # in
        0.05,  # skip
        0.05,  # ink
        0.05,  # skip
      ]

      offset = -0.2

      cr.set_dash(dashes, offset)

      cr.move_to(0.5, 0.1)
      cr.line_to(0.9, 0.9)
      cr.rel_line_to(-0.4, 0.0)
      cr.curve_to(0.2, 0.9, 0.2, 0.5, 0.5, 0.5)

      cr.stroke
    end
  end
end
