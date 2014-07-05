# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-clip-rectangle.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Clip Rectangle

This demo shows how to use GDK and cairo to clip as rectangle.
From http://cairographics.org/samples/xxx_clip_rectangle.html.
=end
require 'common'

module Demo
  class CairoClipRectangle < CairoWindow
    def initialize
      super('cairo clip rectangle')
    end

    def draw(cr)
      cr.new_path
      cr.move_to(0.25, 0.25)
      cr.line_to(0.25, 0.75)
      cr.line_to(0.75, 0.75)
      cr.line_to(0.75, 0.25)
      cr.line_to(0.25, 0.25)
      cr.close_path

      cr.clip

      cr.move_to(0, 0)
      cr.line_to(1, 1)
      cr.stroke
    end
  end
end
