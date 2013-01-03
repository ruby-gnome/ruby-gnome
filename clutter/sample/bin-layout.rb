#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/basic-actor.c. The
# image file used in this sample code is copied from
# clutter/tests/data/redhand.png. They are licensed under the terms
# of the GNU Lesser General Public License, version 2.1 or (at your
# option) later.
#
# Copyright (C) 2012  Ruby-GNOME2 Project Team
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

require "clutter"
require "gdk_pixbuf2"

Clutter.init

bg_color = Clutter::Color.new(0xcc, 0xcc, 0xcc, 0x99)

stage = Clutter::Stage.new
stage.title = "BinLayout"
stage.background_color = Clutter::Color.get_static(:aluminium_2)
stage.set_size(640, 480)
stage.show

stage.signal_connect("destroy") do
  Clutter.main_quit
end

layout = Clutter::BinLayout.new(:center, :center)

box = Clutter::Actor.new
box.layout_manager = layout
box.add_constraint(Clutter::AlignConstraint.new(stage, :both, 0.5))
box.set_position(320, 240)
box.reactive = true
box.name = "box"
stage.add_child(box)

canvas = Clutter::Canvas.new
canvas.signal_connect("draw") do |_canvas, cairo_context, width, height|
  puts("Painting at #{width}x#{height}")

  cairo_context.save do
    cairo_context.operator = :clear
    cairo_context.paint
  end

  bg_round_radius = 12
  x = y = 0

  cairo_context.move_to(bg_round_radius, y)
  cairo_context.line_to(width - bg_round_radius, y)
  cairo_context.curve_to(width, y, width, y, width, bg_round_radius)
  cairo_context.line_to(width, height - bg_round_radius)
  cairo_context.curve_to(width, height, width, height,
                         width - bg_round_radius, height)
  cairo_context.line_to(bg_round_radius, height)
  cairo_context.curve_to(x, height, x, height, x, height - bg_round_radius)
  cairo_context.line_to(x, bg_round_radius)
  cairo_context.curve_to(x, y, x, y, bg_round_radius, y)

  cairo_context.close_path

  cairo_context.set_source_clutter_color(bg_color)
  cairo_context.stroke

  x += 4;
  y += 4;
  width -= 4;
  height -= 4;

  cairo_context.move_to(bg_round_radius, y)
  cairo_context.line_to(width - bg_round_radius, y)
  cairo_context.curve_to(width, y, width, y, width, bg_round_radius)
  cairo_context.line_to(width, height - bg_round_radius)
  cairo_context.curve_to(width, height, width, height,
                         width - bg_round_radius, height)
  cairo_context.line_to(bg_round_radius, height)
  cairo_context.curve_to(x, height, x, height, x, height - bg_round_radius)
  cairo_context.line_to(x, bg_round_radius)
  cairo_context.curve_to(x, y, x, y, bg_round_radius, y)

  cairo_context.close_path

  pattern = Cairo::LinearPattern.new(0, 0, 0, height)
  pattern.add_color_stop_rgba(1, 0.85, 0.85, 0.85, 1)
  pattern.add_color_stop_rgba(0.95, 1, 1, 1, 1)
  pattern.add_color_stop_rgba(0.05, 1, 1, 1, 1)
  pattern.add_color_stop_rgba(0, 0.85, 0.85, 0.85, 1)

  cairo_context.set_source(pattern)
  cairo_context.fill

  true
end
canvas.set_size(200, 200)

bg = Clutter::Actor.new
bg.name = "background"
bg.set_size(200, 200)
bg.content = canvas
bg.x_expand = true
bg.y_expand = true
bg.x_align = :fill
bg.y_align = :fill
box.add_child(bg)
box.signal_connect("transitions-completed") do |actor|
  canvas.set_size(actor.width, actor.height)
end

pixbuf = Gdk::Pixbuf.new(File.expand_path("redhand.png", File.dirname(__FILE__)))
image = Clutter::Image.new
image.set_data(pixbuf.pixels,
               pixbuf.has_alpha? ? :rgba_8888 : :rgb_888,
               pixbuf.width,
               pixbuf.height,
               pixbuf.rowstride)

icon = Clutter::Actor.new
icon.name = "icon"
icon.set_size(196, 196)
icon.x_expand = true
icon.y_expand = true
icon.x_align = :center
icon.y_align = :center
icon.content_gravity = :resize_aspect
icon.set_content_scaling_filters(:trilinear, :linear)
icon.content = image
box.add_child(icon)

color = Clutter::Color.new(rand(255), rand(255), rand(255), 224)
emblem = Clutter::Actor.new
emblem.name = "emblem"
emblem.set_size(48, 48)
emblem.background_color = color
emblem.x_expand = true
emblem.y_expand = true
emblem.x_align :end
emblem.y_align :end
emblem.reactive = true
emblem.opacity = 0
box.add_child(emblem)

action = Clutter::ClickAction.new
emblem.add_action(action)

expanded_p = false
action.signal_connect("clicked") do |_action, _emblem|
  box.save_easing_state do
    box.easing_mode = :ease_out_bounce
    box.easing_duration = 500
    if expanded_p
      box.set_size(200, 200)
    else
      box.set_size(400, 400)
    end
  end
  expanded_p = !expanded_p;
end
action.signal_connect("long-press") do |_action, _emblem, state|
  case state
  when Clutter::LongPressState::QUERY
    puts("*** long press: query ***")
    expanded_p
  when Clutter::LongPressState::CANCEL
    puts("*** long press: cancel ***")
    true
  when Clutter::LongPressState::ACTIVATE
    puts("*** long press: activate ***")
    true
  else
    true
  end
end


box.signal_connect("enter-event") do |_box, event|
  emblem.save_easing_state do
    emblem.easing_mode = :linear
    emblem.opacity = 255
  end

  stop = true
  stop
end
box.signal_connect("leave-event") do
  emblem.save_easing_state do
    emblem.easing_mode = :linear
    emblem.opacity = 0
  end

  stop = true
  stop
end

label = Clutter::Text.new
label.name = "text"
label.text = "A simple test"
label.x_expand = true
label.x_align = :center
label.y_expand = true
label.y_align = :start
box.add_child(label)

Clutter.main
