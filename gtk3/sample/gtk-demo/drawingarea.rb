# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Drawing Area

 GtkDrawingArea is a blank area where you can draw custom displays
 of various kinds.

 This demo has two drawing areas. The checkerboard area shows
 how you can just draw something; all you have to do is write
 a signal handler for "draw", as shown here.

 The "scribble" area is a bit more advanced, and shows how to handle
 events such as button presses and mouse motion. Click the mouse
 and drag in the scribble area to draw squiggles. Resize the window
 to clear the area.
=end
class DrawingareaDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Drawing Area"

    initialize_vbox
    create_the_checkerboard_area
    create_the_scribble_area
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def initialize_vbox
    @vbox = Gtk::Box.new(:vertical, 8)
    @vbox.margin = 16
    @window.add(@vbox)
  end

  def create_da_in_frame(text)
    label = Gtk::Label.new
    label.set_markup(text, :use_underline => true)
    @vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)

    frame = Gtk::Frame.new
    frame.shadow_type = :in
    @vbox.pack_start(frame, :expand => true, :fill => true, :padding => 0)

    da = Gtk::DrawingArea.new
    da.set_size_request(100, 100)
    frame.add(da)
    da
  end

  def checkerboard_draw(da, cr)
    check_size = 10
    spacing = 2
    # At the start of a draw handler, a clip region has been set on
    # the Cairo context, and the contents have been cleared to the
    # widget's background color. The docs for
    # gdk_window_begin_paint_region() give more details on how this
    # works.
    ycount = 0
    xcount = 0
    i = spacing
    while i < da.allocated_width
      j = spacing
      ycount = xcount % 2 # start with even/odd depending on row
      while j < da.allocated_height
        if ycount % 2 == 0
          cr.set_source_rgb(0.45777, 0, 0.45777)
        else
          cr.set_source_rgb(1, 1, 1)
        end
        # if we are outside the clip, this will do nothing
        cr.rectangle(i, j, check_size, check_size)
        cr.fill
        j += check_size + spacing
        ycount += 1
      end
      i += check_size + spacing
      xcount += 1
    end
  end

  def create_the_checkerboard_area
    da = create_da_in_frame("<u>Checkerboard pattern</u>")
    da.signal_connect "draw" do |widget, cr|
      checkerboard_draw(widget, cr)
    end
  end

  def scribble_configure_event(da)
    @surface.destroy if @surface

    @surface = da.window.create_similar_surface(Cairo::CONTENT_COLOR,
                                                da.allocated_width,
                                                da.allocated_height)
    # initialize the surface to white
    cr = Cairo::Context.new(@surface)
    cr.set_source_rgb(1, 1, 1)
    cr.paint
    cr.destroy
    # we have handled the configure event, no need to further processing
    true
  end

  def scribble_draw(cr)
    # Redraw the screen from the surface
    cr.set_source(@surface, 0, 0)
    cr.paint
    false
  end

  def draw_brush(widget, x, y)
    cr = Cairo::Context.new(@surface)
    cr.rectangle(x - 3, y - 3, 6, 6)
    cr.fill
    cr.destroy
    widget.queue_draw_area(x - 3, y - 3, 6, 6)
  end

  def scribble_button_press_event(da, event)
    return false unless @surface
    draw_brush(da, event.x, event.y) if event.button == Gdk::BUTTON_PRIMARY
    true
  end

  def scribble_motion_notify_event(da, event)
    return false unless @surface
    _window, x, y, state = event.window.get_device_position(event.device)
    if (state & Gdk::EventMask::BUTTON_PRESS_MASK.to_i).nonzero?
      draw_brush(da, x, y)
    end
    true
  end

  def create_the_scribble_area
    da = create_da_in_frame("<u>Scribble area</u>")

    da.signal_connect "draw" do |_widget, cr|
      scribble_draw(cr)
    end

    da.signal_connect "configure-event" do |widget|
      scribble_configure_event(widget)
    end

    da.signal_connect "motion-notify-event" do |widget, event|
      scribble_motion_notify_event(widget, event)
    end

    da.signal_connect "button-press-event" do |widget, event|
      scribble_button_press_event(widget, event)
    end

#    da.events |= (Gdk::EventMask::LEAVE_NOTIFY_MASK |
#                    Gdk::EventMask::BUTTON_PRESS_MASK |
#                    Gdk::EventMask::POINTER_MOTION_MASK |
#                    Gdk::EventMask::POINTER_MOTION_HINT_MASK)
    da.add_events([:leave_notify_mask, :button_press_mask,
                   :pointer_motion_mask, :pointer_motion_hint_mask])
  end
end
