#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/canvas.c.
# It is licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
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

stage = Clutter::Stage.new
stage.title = "2D Clock"
stage.user_resizable = true
stage.background_color = Clutter::Color.get_static(:sky_blue_light)
stage.set_size(300, 300)
stage.show

canvas = Clutter::Canvas.new
canvas.set_size(300, 300)

actor = Clutter::Actor.new
actor.content = canvas
actor.set_content_scaling_filters(:trilinear, :linear)
stage.add_child(actor)

actor.add_constraint(Clutter::BindConstraint.new(stage, :size, 0))
idle_resize_id = nil
actor.signal_connect("allocation-changed") do |_actor, allocation, flags|
  idle_resize_id ||= Clutter::Threads.add_timeout(1000) do
    width, height = _actor.size
    _actor.content.set_size(width.ceil, height.ceil)
    idle_resize_id = nil
    GLib::Source::REMOVE
  end
end

stage.signal_connect("destroy") do
  Clutter.main_quit
end

canvas.signal_connect("draw") do |_canvas, cairo_context, width, height|
  now = Time.now
  seconds = now.sec * Math::PI / 30
  minutes = now.min * Math::PI / 30
  hours = now.hour * Math::PI / 6

  cairo_context.save do
    cairo_context.operator = :clear
    cairo_context.paint
  end

  cairo_context.operator = :over
  cairo_context.scale(width, height)

  cairo_context.line_cap = :round
  cairo_context.line_width = 0.1

  # the black rail that holds the seconds indicator
  cairo_context.set_source_clutter_color(Clutter::Color.get_static(:black))
  cairo_context.translate(0.5, 0.5)
  cairo_context.arc(0, 0, 0.4, 0, Math::PI * 2)
  cairo_context.stroke

  # the seconds indicator
  color = Clutter::Color.get_static(:white)
  color.alpha = 128
  cairo_context.set_source_clutter_color(color)
  cairo_context.move_to(0, 0)
  cairo_context.arc(Math.sin(seconds) * 0.4,
                    -Math.cos(seconds) * 0.4,
                    0.05,
                    0,
                    Math::PI * 2)
  cairo_context.fill

  # the minutes hand
  color = Clutter::Color.get_static(:chameleon_dark)
  color.alpha = 196
  cairo_context.set_source_clutter_color(color)
  cairo_context.move_to(0, 0)
  cairo_context.line_to(Math.sin(minutes) * 0.4,
                        -Math.cos(minutes) * 0.4)
  cairo_context.stroke

  # the hours hand
  cairo_context.move_to(0, 0)
  cairo_context.line_to(Math.sin(hours) * 0.2,
                        -Math.cos(hours) * 0.2)
  cairo_context.stroke

  true
end

canvas.invalidate

Clutter::Threads.add_timeout(100, 1000) do
  canvas.invalidate
  GLib::Source::CONTINUE
end

Clutter.main
