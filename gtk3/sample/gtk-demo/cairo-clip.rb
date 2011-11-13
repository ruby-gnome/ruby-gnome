# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-clip.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Clip

This demo shows how to use GDK and cairo to use clip.
From http://cairographics.org/samples/clip.html.
=end
require 'common'

module Demo
  class CairoClip < CairoWindow
    def initialize
      super('cairo clip')
    end

    def draw(cr)
      cr.arc(0.5, 0.5, 0.3, 0, 2 * Math::PI)
      cr.clip

      cr.new_path

      cr.rectangle(0, 0, 1, 1)
      cr.fill
      cr.set_source_rgba(0, 1, 0)
      cr.move_to(0, 0)
      cr.line_to(1, 1)
      cr.move_to(1, 0)
      cr.line_to(0, 1)
      cr.stroke
    end
  end
end
