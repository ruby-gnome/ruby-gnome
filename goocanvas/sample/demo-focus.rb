#!/usr/bin/env ruby
#
# This sample code is a port of
# goocanvas/demo/demo-focus.c. It is licensed
# under the terms of the GNU Library General Public License, version
# 2 or (at your option) later.
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

class CanvasSampleFocus < Gtk::Box
  def initialize
    super(:vertical, 4)
    set_border_width(4)
    show
    create_focus_page(self)
  end

  private
  def create_focus_page(vbox)
    label = Gtk::Label.new(<<-EOT)
Use Tab, Shift+Tab or the arrow keys to move the keyboard focus between the canvas items.
    EOT
    vbox.pack_start(label, :expand  => false,
                           :fill    => false,
                           :padding => 0)
    label.show

    scrolled_win = Gtk::ScrolledWindow.new
    scrolled_win.shadow_type = :in
    scrolled_win.show
    vbox.pack_start(scrolled_win, :expand  => true,
                                  :fill    => true,
                                  :padding => 0)

    canvas = Goo::Canvas.new
    canvas.can_focus = true
    canvas.set_size_request(600, 450)
    canvas.set_bounds(0, 0, 1000, 1000)
    canvas.show
    scrolled_win.add(canvas)

    setup_canvas(canvas)
  end

  def setup_canvas(canvas)
    create_focus_box(canvas, 109,  80, 50, 30, "red")
    create_focus_box(canvas, 300, 160, 50, 30, "orange")
    create_focus_box(canvas, 500,  50, 50, 30, "yellow")
    create_focus_box(canvas,  70, 400, 50, 30, "blue")
    create_focus_box(canvas, 130, 200, 50, 30, "magenta")
    create_focus_box(canvas, 200, 160, 50, 30, "green")
    create_focus_box(canvas, 450, 450, 50, 30, "cyan")
    create_focus_box(canvas, 300, 350, 50, 30, "grey")
    create_focus_box(canvas, 900, 900, 50, 30, "gold")
    create_focus_box(canvas, 800, 150, 50, 30, "thistle")
    create_focus_box(canvas, 600, 800, 50, 30, "azure")
    create_focus_box(canvas, 700, 250, 50, 30, "moccasin")
    create_focus_box(canvas, 500, 100, 50, 30, "cornsilk")
    create_focus_box(canvas, 200, 750, 50, 30, "plum")
    create_focus_box(canvas, 400, 800, 50, 30, "orchid")
  end

  def create_focus_box(canvas, x, y, width, height, color)
    root = canvas.root_item
    item = Goo::CanvasRect.new(:parent         => root,
                               :x              => x,
                               :y              => y,
                               :width          => width,
                               :height         => height,
                               :stroke_pattern => nil,
                               :fill_color     => color,
                               :line_width     => 5.0,
                               :can_focus      => true)

    item.signal_connect("focus-in-event") do |widget|
      on_focus_in(widget, color)
    end
    item.signal_connect("focus-out-event") do |widget|
      on_focus_out(widget, color)
    end

    item.signal_connect("button-press-event") do |widget|
      on_button_press(widget, color)
    end

    item.signal_connect("key-press-event") do
      on_key_press(color)
    end
  end

  def on_focus_in (item, color)
    puts "#{color} received focus-in event"

    # Note that this is only for testing. Setting item properties to indicate
    #   focus isn't a good idea for real apps, as there may be multiple views.
    item.stroke_color = "black"

    false
  end

  def on_focus_out(item, color)
    puts "#{color} received focus-out event"

    # Note that this is only for testing. Setting item properties to indicate
    #   focus isn't a good idea for real apps, as there may be multiple views.
    item.stroke_pattern = nil

    false
  end

  def on_button_press(item, color)
    puts "#{color} received button-press event"
    item.canvas.grab_focus(item)
    false
  end

  def on_key_press(color)
    puts "#{color} received key-press event"
    false
  end
end
