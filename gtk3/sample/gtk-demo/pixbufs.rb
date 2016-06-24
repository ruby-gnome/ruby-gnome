# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Pixbufs

 A GdkPixbuf represents an image, normally in RGB or RGBA format.
 Pixbufs are normally used to load files from disk and perform
 image scaling.

 This demo is not all that educational, but looks cool. It was written
 by Extreme Pixbuf Hacker Federico Mena Quintero. It also shows
 off how to use GtkDrawingArea to do a simple animation.

 Look at the Image demo for additional pixbuf usage examples.
=end
module PixbufsDemo
  IMAGES_NAMES = %w(/pixbufs/apple-red.png /pixbufs/gnome-applets.png
                    /pixbufs/gnome-calendar.png /pixbufs/gnome-foot.png
                    /pixbufs/gnome-gmush.png /pixbufs/gnome-gimp.png
                    /pixbufs/gnome-gsame.png /pixbufs/gnu-keys.png)
  N_IMAGES = IMAGES_NAMES.size
  BACKGROUND_NAME = "/pixbufs/background.jpg"
  CYCLE_TIME = 3_000_000

  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.resizable = false

    background_pixbuf = load_pixbuf(window, BACKGROUND_NAME)
    other_pixbufs = []
    IMAGES_NAMES.each do |img|
      other_pixbufs << load_pixbuf(window, img)
    end

    width = background_pixbuf.width
    height = background_pixbuf.height
    window.set_size_request(width, height)

    frame = GdkPixbuf::Pixbuf.new(:colorspace => :rgb,
                                 :has_alpha => false,
                                 :bits_per_sample => 8,
                                 :width => width, :height => height)

    da = Gtk::DrawingArea.new
    da.signal_connect "draw" do |_widget, cr|
      cr.set_source_pixbuf(frame, 0, 0)
      cr.paint
      true
    end
    window.add(da)
    start_time = 0
    da.add_tick_callback do |_widget, frame_clock|
      background_pixbuf.copy_area(0, 0, width, height, frame, 0, 0)
      start_time = frame_clock.frame_time if start_time == 0
      current_time = frame_clock.frame_time
      f = ((current_time - start_time) % CYCLE_TIME) / Float(CYCLE_TIME)
      xmid = width / 2
      ymid = height / 2
      radius = [xmid, ymid].min / 2
      N_IMAGES.times do |i|
        ang = 2 * Math::PI * i / N_IMAGES - f * 2 * Math::PI
        iw = other_pixbufs[i].width
        ih = other_pixbufs[i].height

        r = radius + (radius / 3) * Math.sin(f * 2 * Math::PI)

        xpos = (xmid + r * Math.cos(ang) - iw / 2 + 0.5).floor
        ypos = (ymid + r * Math.sin(ang) - ih / 2 + 0.5).floor
        k = (i & 1) ? Math.sin(f * 2 * Math::PI) : Math.cos(f * 2 * Math::PI)
        k = 2.0 * k * k
        k = [0.25, k].max

        r1 = Gdk::Rectangle.new(xpos, ypos, iw * k, iw * k)
        r2 = Gdk::Rectangle.new(0, 0, width, height)

        dest = r1.intersect(r2)
        next unless dest
        other_pixbufs[i].composite(frame, dest.x, dest.y, dest.width,
                         dest.height, xpos, ypos, k, k,
                         :nearest,
                         if (i & 1) == 1
                           [
                             127, (255 * Math.sin(f * 2.0 * Math::PI)).abs
                           ].max
                         else
                           [
                             127, (255 * Math.cos(f * 2.0 * Math::PI)).abs
                           ].max
                         end)

      end
      da.queue_draw
      GLib::Source::CONTINUE
    end

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.show_message_dialog_on(window, error)
    message = "Failed to load an image: #{error.message}"
    dialog = Gtk::MessageDialog.new(:parent => window,
                                    :flags => :destroy_with_parent,
                                    :type => :error,
                                    :buttons => :close,
                                    :message => message)
    dialog.signal_connect("response", &:destroy)
    dialog.show
  end

  def self.load_pixbuf(window, image_name)
    begin
      # Is it OK? should we implement gdk_pixbuf_new_from_resource instead?
      #Gtk::Image.new(:resource => image_name).pixbuf
      GdkPixbuf::Pixbuf.new(:resource => image_name)
    rescue StandardError => e
      show_message_dialog_on(window, e)
      window.destroy
    end
  end
end
