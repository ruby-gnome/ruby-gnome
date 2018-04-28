#!/usr/bin/env ruby
#
# Copyright (C) 2018  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

# example from  https://github.com/GNOME/gtk/blob/master/examples/drawing.c

require_relative "utils"

require_gtk4

def clear_surface(surface)
  cr = Cairo::Context.new(surface)
  cr.set_source_rgb(1, 1, 1)
  cr.paint
  cr.destroy
end

# Draw a rectangle on the surface at the given position
def draw_brush(widget, surface, x, y)
  cr = Cairo::Context.new(surface)
  cr.rectangle(x - 3, y - 3, 6, 6)
  cr.fill
  widget.queue_draw_area(x - 3, y - 3, 6, 6)
end

def generate_surface_from_widget(widget)
  surface = nil
  if widget.window
    surface = widget.window.create_similar_surface(Cairo::CONTENT_COLOR,
                                                   widget.allocated_width,
                                                   widget.allocated_height)
  end
  surface
end

myapp = Gtk::Application.new("org.gtk.example", :flags_none)

myapp.signal_connect "activate" do |app|
  win = Gtk::ApplicationWindow.new(app)
  win.set_title("Drawing Area")

  surface = nil

  win.signal_connect "destroy" do
    surface.destroy if surface
    win.destroy
  end

  frame = Gtk::Frame.new
  frame.shadow_type = Gtk::ShadowType::IN
  win.add(frame)

  drawing_area = Gtk::DrawingArea.new
  # Set a minimum size
  drawing_area.set_size_request(100, 100)
  frame.add(drawing_area)

  drawing_area.set_draw_func do |da, cr|
    if surface.nil?
      surface = generate_surface_from_widget(da)
      ## Initialize the surface to white
      clear_surface(surface)
    end
    # Redraw the screen from the surface. Note that the draw
    # callback receives a ready-to-be-used cairo_t that is already
    # clipped to only draw the exposed areas of the widget
    cr.set_source(surface, 0, 0)
    cr.paint
  end

  drawing_area.signal_connect_after "size-allocate" do |da, alloc|
    surface.destroy if surface
    surface = generate_surface_from_widget(da)
    # Initialize the surface to white
    clear_surface(surface) if surface
  end

  drag = Gtk::GestureDrag.new(drawing_area)
  drag.button = Gdk::BUTTON_PRIMARY

  start_x = 0.0
  start_y = 0.0

  drag.signal_connect "drag-begin" do |gesture, x, y|
    start_x = x
    start_y = y
    draw_brush(drawing_area, surface, x, y)
  end

  drag.signal_connect "drag-update" do |gesture, x, y|
    draw_brush(drawing_area, surface, start_x + x, start_y + y)
  end

  drag.signal_connect "drag-end" do |gesture, x, y|
    draw_brush(drawing_area, surface, start_x + x, start_y + y)
  end

  press = Gtk::GestureMultiPress.new(drawing_area)
  press.button = Gdk::BUTTON_SECONDARY
  press.signal_connect "pressed" do |gesture, n_press, x, y|
    clear_surface(surface)
    drawing_area.queue_draw
  end
  win.show
end

myapp.run ARGV
