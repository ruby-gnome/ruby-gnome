# Copyright (c) 2003-2014 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: drawingarea.rb,v 1.5 2005/02/12 23:02:43 kzys Exp $
=begin
= Drawing Area

Gtk::DrawingArea is a blank area where you can draw custom displays
of various kinds.

This demo has two drawing areas. The checkerboard area shows
how you can just draw something; all you have to do is write
a signal handler for expose_event, as shown here.

The 'scribble' area is a bit more advanced, and shows how to handle
events such as button presses and mouse motion. Click the mouse
and drag in the scribble area to draw squiggles. Resize the window
to clear the area.
=end

require "common"

module Demo
  class DrawingArea < BasicWindow
    def initialize
      # Pixmap for scribble area, to store current scribbles
      @cairo_context = nil
      super("Drawing Area")

      self.border_width = 8

      vbox = Gtk::Box.new(:vertical, 8)
      vbox.border_width = 8
      add(vbox)

      ## Create the checkerboard area
      label = Gtk::Label.new
      label.set_markup("<u>Checkerboard pattern</u>")
      vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)

      frame = Gtk::Frame.new
      frame.shadow_type = :in
      vbox.pack_start(frame, :expand => true, :fill => true, :padding => 0)

      da = Gtk::DrawingArea.new
      # set a minimum size
      da.set_size_request(100, 100)

      frame.add(da)

      da.signal_connect("draw") do |widget, event|
        checkerboard_draw(widget, event)
      end

      ## Create the scribble area
      label = Gtk::Label.new
      label.set_markup("<u>Scribble area</u>")
      vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)

      frame = Gtk::Frame.new
      frame.shadow_type = :in
      vbox.pack_start(frame, :expand => true, :fill => true, :padding => 0)

      da = Gtk::DrawingArea.new
      # set a minimum size
      da.set_size_request(100, 100)

      frame.add(da)

      # Signals used to handle backing pixmap
      da.signal_connect("draw") do |*args|
        scribble_draw(*args)
      end
      da.signal_connect("configure-event") do |widget, event|
        scribble_configure_event(widget)
      end

      # Event signals
      da.signal_connect("motion-notify-event") do |*args|
        scribble_motion_notify_event(*args)
      end
      da.signal_connect("button-press-event") do |*args|
        scribble_button_press_event(*args)
      end

      # Ask to receive events the drawing area doesn't normally
      # subscribe to
      da.events |= (Gdk::Event::LEAVE_NOTIFY_MASK |
                    Gdk::Event::BUTTON_PRESS_MASK |
                    Gdk::Event::POINTER_MOTION_MASK |
                    Gdk::Event::POINTER_MOTION_HINT_MASK)
    end

    # Create a new surface of the appropriate size to store our scribbles
    def scribble_configure_event(widget)
      @cairo_context = widget.window.create_cairo_context
      @cairo_context.set_source_rgb(1, 1, 1)
      @cairo_context.paint

      # We've handled the configure event, no need for further processing.
      return true
    end

    CHECK_SIZE = 10
    SPACING = 2
    def checkerboard_draw(da, cairo_context)
      # At the start of a draw handler, a clip region of event.area
      # is set on the window, and event.area has been cleared to the
      # widget's background color. The docs for
      # gdk_window_begin_paint_region give more details on how this
      # works.

      xcount = 0
      width  = da.allocated_width
      height = da.allocated_height
      SPACING.step(da.allocation.width, CHECK_SIZE + SPACING) do |i|
        ycount = xcount % 2; # start with even/odd depending on row
        SPACING.step(da.allocation.height, CHECK_SIZE + SPACING) do |j|
          if ycount % 2 == 1
            cairo_context.set_source_rgb(0.45777, 0, 0.45777)
          else
            cairo_context.set_source_rgb(1, 1, 1)
          end

          # If we're outside the clip, this will do nothing.
          cairo_context.fill do
            cairo_context.rectangle(i, j, CHECK_SIZE, CHECK_SIZE)
          end
          ycount += 1
        end
        xcount += 1
      end
      # return true because we've handled this event, so no
      # further processing is required.
      #
      return true
    end

    # Redraw the screen from the surface
    def scribble_draw(widget, event)
      event.set_source(@cairo_context.source)
      event.paint
      return false
    end

    # Draw a rectangle on the screen
    def draw_brush(widget, x, y)
      update_rect = Gdk::Rectangle.new(x - 3, y - 3, 6, 6)

      # Paint to the surface, where we store our state
      @cairo_context.fill do
        @cairo_context.set_source_rgb(0, 0, 0)
        @cairo_context.rectangle(update_rect.x,
                                 update_rect.y,
                                 update_rect.width,
                                 update_rect.height)
      end

      # Now invalidate the affected region of the drawing area.
      widget.window.invalidate(update_rect, false)
    end

    def scribble_button_press_event(widget, event)
      # paranoia check, in case we haven't gotten a configure event
      return false unless @cairo_context

      if event.button == 1
        draw_brush(widget, event.x, event.y)
      end

      # We've handled the event, stop processing
      return true
    end

    def scribble_motion_notify_event(widget, event)
      # paranoia check, in case we haven't gotten a configure event
      return false unless @cairo_context

      # This call is very important; it requests the next motion event.
      # If you don't call Gdk::Window#pointer you'll only get
      # a single motion event. The reason is that we specified
      # Gdk::POINTER_MOTION_HINT_MASK to Gtk::Widget#set_events.
      # If we hadn't specified that, we could just use event.x, event.y
      # as the pointer location. But we'd also get deluged in events.
      # By requesting the next event as we handle the current one,
      # we avoid getting a huge number of events faster than we
      # can cope.

      # NOTE: Gdk::EventMotion#window will be restored from Ruby/GDK3 2.1.1.
      # win, x, y, state = event.window.get_device_position(event.device)
      win, x, y, state = widget.window.get_device_position(event.device)

      if (state & :button1_mask) != 0
        draw_brush(widget, x, y)
      end

      # We've handled it, stop processing
      return true
    end
  end
end
