# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-image.rb,v 1.2 2005/10/16 12:07:32 mutoh Exp $
=begin
= cairo/Image

This demo shows how to use GDK and cairo to show image.
From http://cairographics.org/samples/image.html.
=end
require 'common'

module Demo
  class CairoImage < CairoWindow
    def initialize
      super('cairo gradient')
    end

    def draw(cr)
      cr.save do
        loader = Gdk::PixbufLoader.new
        File.open("background.jpg", "rb") do |f|
          loader.last_write(f.read)
        end
        pixbuf = loader.pixbuf

        cr.translate(0.5, 0.5)
        cr.rotate(45 * Math::PI / 180)
        cr.scale(1.0 / pixbuf.width, 1.0 / pixbuf.height)
        cr.translate(-0.5 * pixbuf.width, -0.5 * pixbuf.height)

        cr.set_source_pixbuf(pixbuf, 0, 0)
        cr.paint
      end

      cr.save do
        image = Cairo::ImageSurface.from_png("ruby-gnome2-logo.png")

        cr.translate(0.5, 0.5)
        cr.rotate(60 * Math::PI / 180)
        cr.scale(0.8 / image.width, 0.8 / image.height)
        cr.translate(-0.5 * image.width, -0.5 * image.height)

        cr.set_source(image, 0, 0)
        cr.paint
      end
    end
  end
end
