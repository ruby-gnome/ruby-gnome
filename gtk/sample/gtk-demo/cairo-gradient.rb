# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-gradient.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Gradient

This demo shows how to use GDK and cairo to gradient.
From http://cairographics.org/samples/gradient.html.
=end
require 'common'

module Demo
  class CairoGradient < CairoWindow
    def initialize
      super('cairo gradient')
    end

    def draw(cr)
      pat = Cairo::LinearPattern.new(0.0, 0.0,  0.0, 1.0)
      pat.add_color_stop_rgba(1, 0, 0, 0, 1)
      pat.add_color_stop_rgba(0, 1, 1, 1, 1)
      cr.rectangle(0, 0, 1, 1)
      cr.set_source(pat)
      cr.fill
      
      pat = Cairo::RadialPattern.new(0.45, 0.4, 0.1, 0.4, 0.4, 0.5)
      pat.add_color_stop_rgba(0, 1, 1, 1, 1)
      pat.add_color_stop_rgba(1, 0, 0, 0, 1)
      cr.set_source(pat)
      cr.arc(0.5, 0.5, 0.3, 0, 2 * Math::PI)
      cr.fill
    end
  end
end
