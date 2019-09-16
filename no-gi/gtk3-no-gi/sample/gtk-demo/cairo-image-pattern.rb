# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-image-pattern.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Image Pattern

This demo shows how to use GDK and cairo to use image pattern.
From http://cairographics.org/samples/imagepattern.html.
=end
require 'common'

module Demo
  class CairoImagePattern < CairoWindow
    def initialize
      super('cairo image pattern')
    end

    def draw(cr)
      image = Cairo::ImageSurface.from_png("ruby-gnome2-logo.png")

      pattern = Cairo::SurfacePattern.new(image)
      pattern.set_extend(Cairo::EXTEND_REPEAT)

      cr.translate(0.5, 0.5)
      cr.rotate(Math::PI / 4)
      cr.scale(1 / Math.sqrt(2), 1 / Math.sqrt(2))
      cr.translate(-0.5, -0.5)

      matrix = Cairo::Matrix.scale(image.width * 5, image.height * 5)
      pattern.set_matrix(matrix)

      cr.set_source(pattern)

      cr.rectangle(0, 0, 1.0, 1.0)
      cr.fill
    end
  end
end
