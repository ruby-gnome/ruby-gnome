# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-clip-image.rb,v 1.2 2005/10/16 12:07:32 mutoh Exp $
=begin
= cairo/Clip Image

This demo shows how to use GDK and cairo to use clip for image.
From http://cairographics.org/samples/clip_image.html.
=end
require 'common'

module Demo
  class CairoClipImage < CairoWindow
    def initialize
      super('cairo clip image')
    end

    def draw(cr)
      cr.save do
        cr.arc(0.5, 0.5, 0.4, 0, 2 * Math::PI)
        cr.clip

        cr.new_path

        loader = Gdk::PixbufLoader.new
        File.open("background.jpg", "rb") do |f|
          loader.last_write(f.read)
        end
        pixbuf = loader.pixbuf
        cr.scale(1.0 / pixbuf.width, 1.0 / pixbuf.height)

        cr.set_source_pixbuf(pixbuf, 0, 0)
        cr.paint
      end

      cr.save do
        cr.arc(0.5, 0.5, 0.3, 0, 2 * Math::PI)
        cr.clip

        cr.new_path

        image = Cairo::ImageSurface.from_png("ruby-gnome2-logo.png")
        cr.scale(1.0 / image.width, 1.0 / image.height)

        cr.set_source(image, 0, 0)
        cr.paint
      end
    end
  end
end
