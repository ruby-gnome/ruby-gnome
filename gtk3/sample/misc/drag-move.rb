=begin
  drag-move.rb - Move widget by drag sample script.

  Copyright (C) 2006 Kouhei Sutou
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2006/06/17 13:18:12 $
  $Id: drag-move.rb,v 1.2 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

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
    signal_connect("button_press_event") do |widget, event|
      if event.button == @drag_button
        Gtk.grab_add(widget)
        x, y, w, h = widget.allocation.to_a
        drag_start(x, y, event.x_root, event.y_root)
      else
        false
      end
    end
  end

  def set_motion_notify_event
    signal_connect("motion_notify_event") do |widget, event|
      if dragging?
        drag_motion(event.x_root, event.y_root)
      else
        false
      end
    end
  end

  def set_button_release_event
    signal_connect("button_release_event") do |widget, event|
      if event.button == @drag_button
        Gtk.grab_remove(widget)
        drag_end
      else
        false
      end
    end
  end

  def set_drag_move_position_event
    signal_connect("drag_move_position") do |widget, x, y|
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
window.signal_connect("destroy"){Gtk.main_quit}

layout = Gtk::Layout.new

draggable_widget = DraggableWidget.new
draggable_widget.set_size_request(50, 50)
layout.put(draggable_widget, 75, 75)

draggable_widget.signal_connect("expose_event") do |widget, event|
  x, y, w, h = widget.allocation.to_a
  fg = Gdk::GC.new(widget.window)
  widget.window.draw_arc(fg, true, x, y, w, h, 0 * 64, 360 * 64)
  false
end

window.add(layout)

window.show_all

Gtk.main
