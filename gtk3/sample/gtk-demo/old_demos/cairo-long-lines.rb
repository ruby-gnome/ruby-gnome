# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-long-lines.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Long Lines

This demo shows how to use GDK and cairo to show long lines.
From http://cairographics.org/samples/xxx_long_liens.html.
=end
require 'common'

module Demo
  class CairoLongLines < CairoWindow
    def initialize
      super('cairo long lines')
    end

    def draw(cr)
      cr.move_to(0.1, -50)
      cr.line_to(0.1, 50)
      cr.set_source_rgba(1, 0, 0)
      cr.stroke

      cr.move_to(0.2, -60)
      cr.line_to(0.2, 60)
      cr.set_source_rgba(1, 1, 0)
      cr.stroke

      cr.move_to(0.3, -70)
      cr.line_to(0.3, 70)
      cr.set_source_rgba(0, 1, 0)
      cr.stroke

      cr.move_to(0.4, -80)
      cr.line_to(0.4, 80)
      cr.set_source_rgba(0, 0, 1)
      cr.stroke
    end
  end
end
