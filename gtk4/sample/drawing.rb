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

  frame = Gtk::Frame.new
  frame.shadow_type = Gtk::ShadowType::IN
  win.add frame

  drawing_area = Gtk::DrawingArea.new
  # Set a minimum size
  drawing_area.set_size_request 100, 100
  frame.add drawing_area

  # Signals used to handle the backing surface
  drawing_area.set_draw_func do |_da, cr|
    puts _da.class
    puts cr.class
    puts surface.class
    cr.set_source(surface, 0, 0) if surface
    cr.paint
    false
  end

  drawing_area.signal_connect_after "size-allocate" do |da, alloc, baseline, clip|
#  drawing_area.signal_connect "configure-event" do |da, _ev|

    if da.window
      surface.destroy if surface
      surface = da.window.create_similar_surface(Cairo::CONTENT_COLOR,
                                                 da.allocated_width,
                                                 da.allocated_height)
    end
    # Initialize the surface to white
    clear_surface(surface) if surface
#
#    # the configure event have been handled, no need for further
#    # processing
#    true
  end

  # Event signals
#  drawing_area.signal_connect "motion-notify-event" do |da, ev|
#    return false unless surface
#      coords = ev.coords
#
#    if ((ev.state & Gdk::EventMask::BUTTON_PRESS_MASK.to_i) != 0)
#      draw_brush(da, surface, coords[0], coords[1])
#    end
#  end

  drawing_area.signal_connect "button-press-event" do |da, ev|
    return false unless surface

    if (ev.button == Gdk::BUTTON_PRIMARY)
      coords = ev.coords
      draw_brush(da, surface, coords[0], coords[1])
    elsif (ev.button == Gdk::BUTTON_SECONDARY)
      clear_surface(surface)
      da.queue_draw
    end

    true
  end

  win.show
end

myapp.run ARGV
