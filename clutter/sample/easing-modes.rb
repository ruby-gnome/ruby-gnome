#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/easing-mode.c.
# It is licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
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

require "optparse"

require "clutter"

duration = 1

parser = OptionParser.new
parser.on("--duration=SECONDS", Integer,
          "Duration of the animation",
          "(#{duration})") do |_duration|
  duration = _duration
end
parser.parse!

current_mode_index = 0
easing_modes = Clutter::AnimationMode.values.reject do |mode|
  mode == Clutter::AnimationMode::CUSTOM_MODE
end

help_text = lambda do
  format = <<-EOT.chomp
<b>Easing mode: %s (%d of %d)</b>
Left click to tween
Middle click to jump
Right click to change the easing mode
EOT
  current_mode = easing_modes[current_mode_index]
  format % [current_mode.nick, current_mode_index, easing_modes.size]
end

stage = Clutter::Stage.new
stage.title = "Easing Modes"
stage.background_color = Clutter::Color.new(:sky_blue_light)
stage.signal_connect("destroy") do
  Clutter.main_quit
end

main_stage = stage

stage_size = stage.size
stage_width = stage_size.width
stage_height = stage_size.height

bouncer_width = 50
bouncer_height = 50

canvas = Clutter::Canvas.new
canvas.signal_connect("draw") do |_canvas, cairo_context, width, height|
  cairo_context.operator = :clear
  cairo_context.paint

  cairo_context.operator = :over

  radius = [width, height].max

  cairo_context.arc(radius / 2, radius / 2, radius / 2, 0.0, 2.0 * Math::PI)

  bouncer_color = Clutter::Color.new(:scarlet_red_dark)

  pattern = Cairo::RadialPattern.new(radius / 2, radius / 2, 0,
                                     radius, radius, radius)
  pattern.add_color_stop_rgba(0,
                              bouncer_color.red / 255.0,
                              bouncer_color.green / 255.0,
                              bouncer_color.blue / 255.0,
                              bouncer_color.alpha / 255.0)
  pattern.add_color_stop_rgba(0.85,
                              bouncer_color.red / 255.0,
                              bouncer_color.green / 255.0,
                              bouncer_color.blue / 255.0,
                              0.25);

  cairo_context.set_source(pattern)
  cairo_context.fill

  Clutter::Event::STOP
end
canvas.set_size(bouncer_width, bouncer_height)

rectangle = Clutter::Actor.new
rectangle.name = "bouncer"
rectangle.set_size(bouncer_width, bouncer_height)
rectangle.set_pivot_point(0.5, 0.5)
rectangle.set_translation(bouncer_width / -2.0, bouncer_height / -2.0, 0.0)
rectangle.reactive = true
rectangle.content = canvas

stage.add_child(rectangle)
rectangle.set_position(stage_width / 2, stage_height / 2)

label = Clutter::Text.new
stage.add_child(label)
label.markup = help_text.call
label.line_alignment = :right
label.add_constraint(Clutter::AlignConstraint.new(stage, :x_axis, 0.95))
label.add_constraint(Clutter::AlignConstraint.new(stage, :y_axis, 0.95))
easing_mode_label = label

stage.signal_connect("button-press-event") do |actor, event|
  case event.button
  when Clutter::BUTTON_SECONDARY
    current_mode_index = (current_mode_index + 1) % easing_modes.size
    label.markup = help_text.call
  when Clutter::BUTTON_MIDDLE
    rectangle.set_position(event.x, event.y)
  when Clutter::BUTTON_PRIMARY
    current_mode = easing_modes[current_mode_index]
    rectangle.save_easing_state do
      rectangle.easing_mode = current_mode
      rectangle.easing_duration = duration * 1000

      rectangle.set_position(event.x, event.y)
    end
  end

  Clutter::Event::STOP
end

stage.show

Clutter.main
