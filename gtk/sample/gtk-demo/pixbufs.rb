# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: pixbufs.rb,v 1.5 2005/02/12 23:02:43 kzys Exp $
=begin
= Pixbufs

A Gdk::Pixbuf represents an image, normally in RGB or RGBA format.
Pixbufs are normally used to load files from disk and perform
image scaling.

This demo is not all that educational, but looks cool. It was written
by Extreme Pixbuf Hacker Federico Mena Quintero. It also shows
off how to use Gtk::DrawingArea to do a simple animation.

Look at the Image demo for additional pixbuf usage examples.
=end

require 'common'

module Demo
  class Pixbufs < BasicWindow
    FRAME_DELAY = 50

    BACKGROUND_NAME = 'background.jpg'

    IMAGE_NAMES = [
      'apple-red.png',
      'gnome-applets.png',
      'gnome-calendar.png',
      'gnome-foot.png',
      'gnome-gmush.png',
      'gnome-gimp.png',
      'gnome-gsame.png',
      'gnu-keys.png',
      'ruby-gnome2-logo.png'
    ]

    CYCLE_LEN = 60
    def initialize
      super('Pixbufs')
      set_resizable(false)

      @background = nil
      @frame = nil
      @frame_num = 0
      @images = []

      begin
	load_pixbufs

	set_size_request(@background.width, @background.height)

	@frame = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB,
				 false, 8,
				 @background.width, @background.height)

	@da = Gtk::DrawingArea.new

	@da.signal_connect('expose_event') do |w, e|
	  expose_cb(w, e)
	end

	add(@da)

	timeout_id = Gtk.timeout_add(FRAME_DELAY) do
          timeout
	end
        signal_connect('destroy') do
          Gtk.timeout_remove(timeout_id)
        end
      rescue
	dialog = Gtk::MessageDialog.new(self,
					Gtk::Dialog::DESTROY_WITH_PARENT,
					Gtk::MessageDialog::ERROR,
					Gtk::MessageDialog::BUTTONS_CLOSE,
					"Failed to load an image: #{$!.message}")

	dialog.signal_connect('response') do
	  dialog.destroy
	end

	dialog.show
      end
    end

    def load_pixbufs
      # Loads the images for the demo

      if @background
	return # already loaded earlier
      end

      # demo_find_file() looks in the the current directory first,
      # so you can run gtk-demo without installing GTK, then looks
      # in the location where the file is installed.
      #
      filename = Demo.find_file(BACKGROUND_NAME)
      @background = Gdk::Pixbuf.new(filename)

      IMAGE_NAMES.each_with_index do |basename, i|
	filename = Demo.find_file(basename)

	@images[i] = Gdk::Pixbuf.new(filename)
      end
    end

    def expose_cb(widget, event)
      rowstride = @frame.rowstride

      pixels = @frame.pixels
      pixels[0, rowstride * event.area.y + event.area.x * 3] = ''

      Gdk::RGB.draw_rgb_image(widget.window,
                              widget.style.black_gc,
                              event.area.x, event.area.y,
                              event.area.width, event.area.height,
                              Gdk::RGB::Dither::NORMAL,
                              pixels, rowstride,
                              event.area.x, event.area.y)
      true
    end

    # Timeout handler to regenerate the frame
    def timeout
      @background.copy_area(0, 0, @background.width, @background.height,
                           @frame, 0, 0)

      f = Float(@frame_num % CYCLE_LEN) / CYCLE_LEN;

      xmid = @background.width / 2.0
      ymid = @background.height / 2.0

      radius = [xmid, ymid].min / 2.0

      @images.each_with_index do |image, i|
        ang = 2.0 * Math::PI * Float(i) / IMAGE_NAMES.length - f * 2.0 * Math::PI

        r = radius + (radius / 3.0) * Math.sin(f * 2.0 * Math::PI)

        xpos = (xmid + r * Math.cos(ang) - image.width / 2.0 + 0.5).floor
        ypos = (ymid + r * Math.sin(ang) - image.height / 2.0 + 0.5).floor

        k = if (i & 1) == 1
              Math.sin(f * 2.0 * Math::PI)
            else
              Math.cos(f * 2.0 * Math::PI)
            end
        k = [0.25, 2.0 * k * k].max

        r1 = Gdk::Rectangle.new(xpos, ypos, image.width * k, image.height * k)
        r2 = Gdk::Rectangle.new(0, 0, @background.width, @background.height)

        dest = r1.intersect(r2)
        if dest
          @frame.composite!(image, dest.x, dest.y, dest.width, dest.height,
                            xpos, ypos, k, k, Gdk::Pixbuf::INTERP_NEAREST,
                            if (i & 1) == 1
                              [
                                127,
                                (255 * Math.sin(f * 2.0 * Math::PI)).abs
                              ].max
                            else
                              [
                                127,
                                (255 * Math.cos(f * 2.0 * Math::PI)).abs
                              ].max
                            end)
        end
      end
      @da.queue_draw
      @frame_num += 1
      true
    end
  end
end
