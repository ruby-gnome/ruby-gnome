#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/box-layout.c.
# The original header:
#   Copyright 2009 Intel Corporation.
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
# Copyright (C) 2012  Ruby-GNOME2 Project Team
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

require "clutter"

Clutter.init

stage = Clutter::Stage.new
stage.title = "Box Layout"
stage.user_resizable = true

layout = Clutter::BoxLayout.new
layout.orientation = :vertical
stage.layout_manager = layout

box = Clutter::Actor.new
box.background_color = Clutter::Color.get_static(:light_gray)
box.x_expand = true
box.y_expand = true
layout = Clutter::BoxLayout.new
box.layout_manager = layout
stage.add_child(box)

instructions_label = [
  "Press v\t\342\236\236\tSwitch horizontal/vertical",
  "Press h\t\342\236\236\tToggle homogeneous",
  "Press p\t\342\236\236\tToggle pack start/end",
  "Press s\t\342\236\236\tIncrement spacing (up to 12px)",
  "Press +\t\342\236\236\tAdd a new actor",
  "Press a\t\342\236\236\tToggle animations",
  "Press q\t\342\236\236\tQuit"
].join("\n")
instructions = Clutter::Text.new("Sans 12px", instructions_label)
instructions.x_expand = true
instructions.y_expand = false
instructions.x_align = :start
instructions.margin_top = 4
instructions.margin_left = 4
instructions.margin_bottom = 4
stage.add_child(instructions)

add_actor = lambda do |_box, position|
  color = Clutter::Color.hls(rand * 360, 0.5, 0.5)

  layout = Clutter::BinLayout.new(:center, :center)
  rect = Clutter::Actor.new
  rect.layout_manager = layout
  rect.background_color = color
  rect.reactive = true
  rect.set_size(32, 64)
  rect.x_expand = true
  rect.y_expand = true
  rect.x_align = :center
  rect.y_align = :center

  text = Clutter::Text.new("Sans 8px", "")
  text.line_alignment = :center
  rect.add_child(text)

  rect.signal_connect("button-release-event") do |_rect, event|
    x_align = _rect.x_align
    y_align = _rect.y_align
    x_expand = _rect.x_expand?
    y_expand = _rect.y_expand?

    case event.button
    when Clutter::BUTTON_PRIMARY
      aligns = Clutter::ActorAlign.values
      if event.has_shift_modifier?
        y_align = aligns[(aligns.index(y_align) + 1) % aligns.size]
      else
        x_align = aligns[(aligns.index(x_align) + 1) % aligns.size]
      end
    when Clutter::BUTTON_SECONDARY
      if event.has_shift_modifier?
        y_expand = !y_expand
      else
        x_expand = !x_expand
      end
    end

    _rect.x_align = x_align
    _rect.y_align = y_align
    _rect.x_expand = x_expand
    _rect.y_expand = y_expand

    true
  end

  changed = lambda do |actor, pspec|
    x_align = actor.x_align
    y_align = actor.y_align
    x_expand = actor.x_expand?
    y_expand = actor.y_expand?

    label = ["#{x_expand},#{y_expand}", x_align.nick, y_align.nick].join("\n")
  end
  rect.signal_connect("notify::x-align", &changed)
  rect.signal_connect("notify::y-align", &changed)
  rect.signal_connect("notify::x-expand", &changed)
  rect.signal_connect("notify::y-expand", &changed)
  changed.call(rect, nil)

  _box.insert_child_at_index(rect, position)
end

5.times do |i|
  add_actor.call(box, i)
end

stage.signal_connect("destroy") do
  Clutter.main_quit
end

stage.signal_connect("key-release-event") do |_stage, event|
  layout = box.layout_manager

  handled = true
  case event.key_symbol
  when Clutter::Keys::KEY_a
    iter = Clutter::ActorIter.new(box)
    iter.each do |child|
      duration = child.easing_duration
      if duration.zero?
        duration = 250
      else
        duration = 0
      end
      child.easing_duration = duration
    end
  when Clutter::Keys::KEY_v
    orientation = layout.orientation
    if orientation == Clutter::Orientation::HORIZONTAL
      orientation = :vertical
    else
      orientation = :horizontal
    end
    layout.orientation = orientation
  when Clutter::Keys::KEY_h
    layout.homogeneous = !layout.homogeneous?
  when Clutter::Keys::KEY_p
    layout.pack_start = !layout.pack_start?
  when Clutter::Keys::KEY_s
    spacing = layout.spacing
    if spacing > 12
      spacing = 0
    else
      spacing += 1
    end
    layout.spacing = spacing
  when Clutter::Keys::KEY_plus
    add_actor.call(box, rand(box.n_children))
  when Clutter::Keys::KEY_q
    Clutter.main_quit
  else
    handled = false
  end

  handled
end

stage.show

Clutter.main
