#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/grid-layout.c.
# The original header:
#   Copyright 2012 Bastian Winkler <buz@netbuz.org>
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms and conditions of the GNU Lesser General Public License,
#   version 2.1, as published by the Free Software Foundation.
#
#   This program is distributed in the hope it will be useful, but WITHOUT ANY
#   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
#   more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with this program; if not, write to the Free Software Foundation,
#   Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
#   Boston, MA 02111-1307, USA.
#
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License, version 2.1, as published by the Free Software Foundation.
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

random_size_p    = false
random_align_p   = false
default_expand_p = true
use_box_p        = false
vertical_p       = false

parser = OptionParser.new
parser.on("--[no-]random-size",
          "Randomly size the rectangles",
          "(#{random_size_p})") do |boolean|
  random_size_p = boolean
end
parser.on("--[no-]random-align",
          "Randomly set the align values",
          "(#{random_align_p})") do |boolean|
  random_align_p = boolean
end
parser.on("--no-expand",
          "Don't expand all actors by default",
          "(#{default_expand_p})") do |boolean|
  default_expand_p = boolean
end
parser.on("--[no-]box",
          "Use the layout in a Clutter::BoxLayout style",
          "(#{use_box_p})") do |boolean|
  use_box_p = boolean
end
parser.on("--[no-]vertical",
          "Use a vertical orientation when used with --box",
          "(#{vertical_p})") do |boolean|
  vertical_p = boolean
end
parser.parse!

stage = Clutter::Stage.new
stage.user_resizable = true

stage_layout = Clutter::BoxLayout.new
stage_layout.orientation = :vertical
stage.layout_manager = stage_layout

grid_layout = Clutter::GridLayout.new
if vertical_p
  grid_layout.orientation = :vertical
end

box = Clutter::Actor.new
box.background_color = Clutter::Color.new(:light_gray)
box.x_expand = true
box.y_expand = true
box.layout_manager = grid_layout
stage_layout.pack(box, true, true, true, :center, :center)

add_actor = lambda do |left, top, width, height|
  color = Clutter::Color.hls(rand(0.0...360.0),
                             0.5,
                             0.5)
  color.alpha = 255

  layout = Clutter::BinLayout.new(:center, :center)
  rect = Clutter::Actor.new
  rect.layout_manager = layout
  rect.background_color = color
  rect.reactive = true

  if random_size_p
    rect.set_size(rand(40..80),
                  rand(40..80))
  else
    rect.set_size(60, 60)
  end
  rect.x_expand = default_expand_p
  rect.y_expand = default_expand_p

  unless default_expand_p
    rect.x_align = :center
    rect.y_align = :center
  end

  if random_align_p
    aligns = Clutter::ActorAlign.values
    rect.x_align = aligns[rand(aligns.size)]
    rect.y_align = aligns[rand(aligns.size)]
  end

  text = Clutter::Text.new("Sans 8px", "")
  text.line_alignment = :center
  rect.add_child(text)

  changed = lambda do |actor, pspec|
    layout = box.layout_manager
    meta = layout.get_child_meta(box, actor)

    label = <<-EOL.chomp
attach: #{meta.left_attach},#{meta.top_attach}
span: #{meta.width},#{meta.height}
expand: #{actor.x_expand?},#{actor.y_expand?}
align: #{actor.x_align.nick},#{actor.y_align.nick}"
EOL
    text.text = label
  end

  rect.signal_connect("button-release-event") do |actor, event|
    x_align = actor.x_align
    y_align = actor.y_align
    x_expand_p = actor.x_expand?
    y_expand_p = actor.y_expand?

    processed = true
    aligns = Clutter::ActorAlign.values
    case event.button
    when Clutter::BUTTON_PRIMARY
      if event.has_shift_modifier?
        actor.x_expand = !actor.x_expand?
      else
        actor.x_align = aligns[(actor.x_align.to_i + 1) % aligns.size]
      end
    when Clutter::BUTTON_SECONDARY
      if event.has_shift_modifier?
        actor.y_expand = !actor.y_expand?
      else
        actor.y_align = aligns[(actor.y_align.to_i + 1) % aligns.size]
      end
    else
      processed = false
    end
    processed
  end
  rect.signal_connect("notify::x-expand", &changed)
  rect.signal_connect("notify::y-expand", &changed)
  rect.signal_connect("notify::x-align",  &changed)
  rect.signal_connect("notify::y-align",  &changed)

  layout = box.layout_manager
  if use_box_p
    box.add_child(rect)
  else
    layout.attach(rect, left, top, width, height)
  end
  changed.call(rect, nil)
end

add_actor.call(0, 0, 1, 1)
add_actor.call(1, 0, 1, 1)
add_actor.call(2, 0, 1, 1)
add_actor.call(0, 1, 1, 1)
add_actor.call(1, 1, 2, 1)
add_actor.call(0, 2, 3, 1)
add_actor.call(0, 3, 2, 2)
add_actor.call(2, 3, 1, 1)
add_actor.call(2, 4, 1, 1)

instructions = Clutter::Text.new("Sans 12px", <<-EOI.chomp)
Press r\t\342\236\236\tSwitch row homogeneous
Press c\t\342\236\236\tSwitch column homogeneous
Press s\t\342\236\236\tIncrement spacing (up to 12px)
Press q\t\342\236\236\tQuit

Left/right click\t\t\342\236\236\tChange actor align
Shift left/right click\t\342\236\236\tChange actor expand
EOI
instructions.margin_top    = 4
instructions.margin_left   = 4
instructions.margin_bottom = 4
stage_layout.pack(instructions, false, true, false, :start, :center)

stage.signal_connect("destroy") do
  Clutter.main_quit
end

stage.signal_connect("key-release-event") do |actor, event|
  layout = box.layout_manager

  processed = true
  case event.key_symbol
  when Clutter::Keys::KEY_c
    homogeneous_p = layout.column_homogeneous?
    layout.column_homogeneous = !homogeneous_p
  when Clutter::Keys::KEY_r
    homogeneous_p = layout.row_homogeneous?
    layout.row_homogeneous = !homogeneous_p
  when Clutter::Keys::KEY_s
    spacing = (layout.column_spacing + 1) % 12
    layout.column_spacing = spacing
    layout.row_spacing    = spacing
  when Clutter::Keys::KEY_q
    Clutter.main_quit
  else
    processed = false
  end

  processed
end

stage.show

Clutter.main
