# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-path.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Path

This demo shows how to use GDK and cairo to draw path.
From http://cairographics.org/samples/path.html.
=end
require 'common'

module Demo
  class CairoPath < CairoWindow
    def initialize
      super('cairo path')
    end

    def draw(cr)
      cr.move_to(0.5, 0.1)
      cr.line_to(0.9, 0.9)
      cr.rel_line_to(-0.4, 0.0)
      cr.curve_to(0.2, 0.9, 0.2, 0.5, 0.5, 0.5)

      cr.stroke
    end
  end
end
