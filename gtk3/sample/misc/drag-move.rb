=begin
  drag-move.rb - Move widget by drag sample script.

  Copyright (C) 2011-2015 Ruby-GNOME2 Project Team
  Copyright (C) 2006 Kouhei Sutou
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

class DraggableWidget < Gtk::EventBox
  def initialize
    super
    set_visible_window(false)
    @dragging = false
    @drag_button = 1
    set_button_press_event
    set_button_release_event
    set_motion_notify_event
  end

  def layout
    parent
  end

  def move(x, y)
    if layout
      layout.move(self, x, y)
      true
    else
      false
    end
  end

  def dragging?
    @dragging
  end

  private
  def set_button_press_event
    signal_connect("button-press-event") do |widget, event|
      if event.button == @drag_button
        widget.grab_add
        x, y, w, h = widget.allocation.to_a
        drag_start(x, y, event.x_root, event.y_root)
      else
        false
      end
    end
  end

  def set_motion_notify_event
    signal_connect("motion-notify-event") do |widget, event|
      if dragging?
        drag_motion(event.x_root, event.y_root)
      else
        false
      end
    end
  end

  def set_button_release_event
    signal_connect("button-release-event") do |widget, event|
      if event.button == @drag_button
        widget.grab_remove
        drag_end
      else
        false
      end
    end
  end

  def set_drag_move_position_event
    signal_connect("drag-move-position") do |widget, x, y|
      if layout
        layout.move(widget, x, y)
        true
      else
        false
      end
    end
  end

  def drag_start(x, y, base_x, base_y)
    @dragging = true
    @drag_x = x
    @drag_y = y
    @drag_base_x = base_x
    @drag_base_y = base_y
    true
  end

  def drag_motion(base_x, base_y)
    delta_x = base_x - @drag_base_x
    delta_y = base_y - @drag_base_y
    if delta_x != 0 and delta_y != 0
      move(@drag_x + delta_x, @drag_y + delta_y)
    else
      false
    end
  end

  def drag_end
    @dragging = false
    true
  end
end

window = Gtk::Window.new("Draggable Widget sample")
window.signal_connect("destroy") {Gtk.main_quit}

layout = Gtk::Layout.new

draggable_widget = DraggableWidget.new
draggable_widget.set_size_request(50, 50)
layout.put(draggable_widget, 75, 75)

draggable_widget.signal_connect("draw") do |widget, context|
  width  = widget.width_request
  height = widget.height_request
  context.fill do
    center_x = width / 2
    center_y = height / 2
    radius   = width / 2
    start_angle = 0 * Math::PI
    stop_angle  = 2 * Math::PI
    context.arc(center_x, center_y, radius, start_angle, stop_angle)
  end
  false
end

window.add(layout)

window.show_all

Gtk.main
