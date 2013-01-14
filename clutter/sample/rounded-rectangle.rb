#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/rounded-rectangle.c.
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

Clutter.init

stage = Clutter::Stage.new
stage.title = "Rectangle with rounded corners"
stage.background_color = Clutter::Color.new(:black)
stage.set_size(500, 500)
stage.show

canvas = Clutter::Canvas.new
canvas.set_size(300, 300)

actor = Clutter::Actor.new
actor.content = canvas
actor.content_gravity = :center
actor.set_content_scaling_filters(:trilinear, :linear)
actor.set_pivot_point(0.5, 0.5)
actor.add_constraint(Clutter::BindConstraint.new(stage, :size, 0.0))
stage.add_child(actor)

transition = Clutter::PropertyTransition.new("rotation-angle-y")
transition.from = 0.0
transition.to = 360.0
transition.duration = 2000
transition.repeat_count = -1
actor.add_transition("rotateActor", transition)

stage.signal_connect("destroy") do
  Clutter.main_quit
end

canvas.signal_connect("draw") do |_canvas, cairo_context, surface_width, surface_height|
  x             = 1.0
  y             = 1.0
  width         = surface_width - 2.0
  height        = surface_height - 2.0
  aspect        = 1.0
  corner_radius = height / 20.0

  radius = corner_radius / aspect
  degrees = Math::PI / 180.0

  cairo_context.save do
    cairo_context.operator = :clear
    cairo_context.paint
  end

  cairo_context.new_sub_path
  cairo_context.arc(x + width - radius, y + radius, radius,
                    -90 * degrees, 0 * degrees)
  cairo_context.arc(x + width - radius, y + height - radius,
                    radius, 0 * degrees, 90 * degrees)
  cairo_context.arc(x + radius, y + height - radius, radius,
                    90 * degrees, 180 * degrees)
  cairo_context.arc(x + radius, y + radius, radius,
                    180 * degrees, 270 * degrees)
  cairo_context.close_path

  cairo_context.set_source_rgb(0.5, 0.5, 1)
  cairo_context.fill

  Clutter::Event::STOP
end

canvas.invalidate

Clutter.main
