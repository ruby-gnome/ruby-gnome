=begin
  drawing.rb - Ruby/GTK version of gtk+/examples/drawing.c.
  https://git.gnome.org/browse/gtk+/tree/examples/drawing.c?h=gtk-3-16
  Copyright (c) 2015-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "gtk3"

surface = nil

def clear_surface(surface)
  cr = Cairo::Context.new(surface)
  cr.set_source_rgb(1, 1, 1)
  cr.paint
  cr.destroy
end

def draw_brush(widget, surface, x, y)
  cr = Cairo::Context.new(surface)
  cr.rectangle(x - 3, y - 3, 6, 6)
  cr.fill
  cr.destroy
  widget.queue_draw_area(x - 3, y - 3, 6, 6)
end

myapp = Gtk::Application.new "org.gtk.example", :flags_none

myapp.signal_connect "activate" do |app|
  win = Gtk::ApplicationWindow.new app
  win.set_title "Drawing Area"

  win.signal_connect "delete-event" do
    win.destroy
  end

  win.set_border_width 8

  frame = Gtk::Frame.new
  frame.shadow_type = Gtk::ShadowType::IN
  win.add frame

  drawing_area = Gtk::DrawingArea.new
  # Set a minimum size
  drawing_area.set_size_request 100, 100
  frame.add drawing_area

  # Signals used to handle the backing surface
  drawing_area.signal_connect "draw" do |_da, cr|
    cr.set_source(surface, 0, 0)
    cr.paint
    false
  end

  drawing_area.signal_connect "configure-event" do |da, _ev|
    surface.destroy if surface
    surface = win.window.create_similar_surface(Cairo::CONTENT_COLOR,
                                                da.allocated_width,
                                                da.allocated_height)
    # Initialize the surface to white
    clear_surface(surface)

    # the configure event have been handled, no need for further
    # processing
    true
  end

  # Event signals
  drawing_area.signal_connect "motion-notify-event" do |da, ev|
    return false unless surface

    if (ev.state & Gdk::EventMask::BUTTON_PRESS_MASK.to_i) != 0
      draw_brush(da, surface, ev.x, ev.y)
    end
  end

  drawing_area.signal_connect "button-press-event" do |da, ev|
    return false unless surface

    case ev.button
    when Gdk::BUTTON_PRIMARY
      draw_brush(da, surface, ev.x, ev.y)
    when Gdk::BUTTON_SECONDARY
      clear_surface(surface)
      da.queue_draw
    end

    true
  end
  # Ask to receive events the drawing area doesn't normally
  # subscribe to. In particular, we need to ask for the
  # button press and motion notify events that we want to handle.
  drawing_area.add_events([:button_press_mask, :pointer_motion_mask])

  win.show_all
end

myapp.run ARGV
