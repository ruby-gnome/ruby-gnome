# $Id: drawingarea.rb,v 1.2 2003/03/21 13:59:46 mutoh Exp $
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
require 'common'

module Demo
  class DrawingArea < BasicWindow
    def initialize
      # Pixmap for scribble area, to store current scribbles
      @pixmap = nil
      super('Drawing Area')

      self.border_width = 8

      vbox = Gtk::VBox.new(false, 8)
      vbox.border_width = 8
      add(vbox)

      
      ## Create the checkerboard area
      label = Gtk::Label.new
      label.set_markup('<u>Checkerboard pattern</u>')
      vbox.pack_start(label, false, false, 0)
      
      frame = Gtk::Frame.new
      frame.shadow_type = Gtk::SHADOW_IN
      vbox.pack_start(frame, true, true, 0)
      
      da = Gtk::DrawingArea.new
      # set a minimum size
      da.set_size_request(100, 100)

      frame.add(da)

      da.signal_connect('expose_event') do |widget, event|
	checkerboard_expose(widget)
      end

      ## Create the scribble area
      label = Gtk::Label.new
      label.set_markup('<u>Scribble area</u>')
      vbox.pack_start(label, false, false, 0)
      
      frame = Gtk::Frame.new
      frame.shadow_type = Gtk::SHADOW_IN
      vbox.pack_start(frame, true, true, 0)
      
      da = Gtk::DrawingArea.new
      # set a minimum size
      da.set_size_request(100, 100)

      frame.add(da)

      # Signals used to handle backing pixmap
      da.signal_connect('expose_event') do |*args|
	scribble_expose_event(*args)
      end
      da.signal_connect('configure_event') do |widget, event|
	scribble_configure_event(widget)
      end
      
      # Event signals
      da.signal_connect('motion_notify_event') do |*args|
	scribble_motion_notify_event(*args)
      end
      da.signal_connect('button_press_event') do |*args|
	scribble_button_press_event(*args)
      end
      
      # Ask to receive events the drawing area doesn't normally
      # subscribe to
      da.events |= (Gdk::Event::LEAVE_NOTIFY_MASK |
		    Gdk::Event::BUTTON_PRESS_MASK |
		    Gdk::Event::POINTER_MOTION_MASK |
		    Gdk::Event::POINTER_MOTION_HINT_MASK)
    end

    # Create a new pixmap of the appropriate size to store our scribbles
    def scribble_configure_event(widget)
      @pixmap = Gdk::Pixmap.new(self.window,
				widget.allocation.width,
				widget.allocation.height,
				-1)

      # Initialize the pixmap to white
      @pixmap.draw_rectangle(widget.style.white_gc,
			     true,
			     0, 0,
			     widget.allocation.width,
			     widget.allocation.height)

      # We've handled the configure event, no need for further processing.
      return true
    end

    CHECK_SIZE = 10
    SPACING = 2
    def checkerboard_expose(da)
      # At the start of an expose handler, a clip region of event.area
      # is set on the window, and event.area has been cleared to the
      # widget's background color. The docs for
      # gdk_window_begin_paint_region give more details on how this
      # works.
  

      # It would be a bit more efficient to keep these
      # GC's around instead of recreating on each expose, but
      # this is the lazy/slow way.
      gc1 = Gdk::GC.new(da.window)
      gc1.rgb_fg_color = Gdk::Color.new(30000, 0, 30000)
      
      gc2 = Gdk::GC.new(da.window)
      gc2.rgb_fg_color = Gdk::Color.new(65535, 65535, 65535)

      xcount = 0
      SPACING.step(da.allocation.width, CHECK_SIZE + SPACING) do |i|
	ycount = xcount % 2; # start with even/odd depending on row
	SPACING.step(da.allocation.height, CHECK_SIZE + SPACING) do |j|
	  gc = if ycount % 2 == 1
		 gc1
	       else
		 gc2
	       end
	  
	  # If we're outside event.area, this will do nothing.
	  # It might be mildly more efficient if we handled
	  # the clipping ourselves, but again we're feeling lazy.

	  da.window.draw_rectangle(gc, true, i, j, CHECK_SIZE, CHECK_SIZE)
	  ycount += 1
	end
	xcount += 1
      end
      # return true because we've handled this event, so no
      # further processing is required.
      #
      return true
    end

    # Redraw the screen from the pixmap
    def scribble_expose_event(widget, event)
      # We use the 'foreground GC' for the widget since it already exists,
      # but honestly any GC would work. The only thing to worry about
      # is whether the GC has an inappropriate clip region set.
      widget.window.draw_drawable(widget.style.fg_gc(widget.state),
				  @pixmap,
				  # Only copy the area that was exposed.
				  event.area.x, event.area.y,
				  event.area.x, event.area.y,
				  event.area.width, event.area.height)
      return false
    end

  

    def draw_brush(widget, x, y)
      update_rect = Gdk::Rectangle.new(x - 3, y - 3, 6, 6)

      # Paint to the pixmap, where we store our state
      @pixmap.draw_rectangle(widget.style.black_gc,
			    true,
			    update_rect.x, update_rect.y,
			    update_rect.width, update_rect.height)

      # Now invalidate the affected region of the drawing area.
      widget.window.invalidate(update_rect, false)
    end

    def scribble_button_press_event(widget, event)
      unless @pixmap
	# paranoia check, in case we haven't gotten a configure event
	return false
      end
  
      if event.button == 1
	draw_brush(widget, event.x, event.y)
      end

      # We've handled the event, stop processing
      return true
    end

    def scribble_motion_notify_event(widget, event)
      unless @pixmap
	# paranoia check, in case we haven't gotten a configure event
	return false
      end

      # This call is very important; it requests the next motion event.
      # If you don't call Gdk::Window#pointer you'll only get
      # a single motion event. The reason is that we specified
      # Gdk::POINTER_MOTION_HINT_MASK to Gtk::Widget#set_events.
      # If we hadn't specified that, we could just use event.x, event.y
      # as the pointer location. But we'd also get deluged in events.
      # By requesting the next event as we handle the current one,
      # we avoid getting a huge number of events faster than we
      # can cope.
      
      x, y, state = event.window.pointer

      if (state & Gdk::Window::BUTTON1_MASK) != 0
	draw_brush(widget, x, y)
      end

      # We've handled it, stop processing
      return true
    end
  end
end
