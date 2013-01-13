#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/drag-action.c.
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

Clutter.init

drag_axis = :none
x_drag_threshold = 0
y_drag_threshold = 0

parser = OptionParser.new
parser.on("--x-threshold=PIXELS", Integer,
          "Set the horizontal drag threshold",
          "(#{x_drag_threshold})") do |pixels|
  x_drag_threshold = pixels
end
parser.on("--y-threshold=PIXELS", Integer,
          "Set the vertical drag threshold",
          "(#{y_drag_threshold})") do |pixels|
  y_drag_threshold = pixels
end
axises = {
  :none => :axis_none,
  :x    => :x_axis,
  :y    => :y_axis,
}
parser.on("--axis=AXIS", axises.keys,
          "Set the drag axis",
          "[#{axises.keys.join(', ')}]",
          "(#{drag_axis})") do |axis|
  drag_axis = axis
end
parser.parse!

drag_axis = axises[drag_axis]

stage = Clutter::Stage.new
stage.title = "Drag Test"
stage.set_size(800, 600)
stage.signal_connect("destroy") do
  Clutter.main_quit
end

handle = Clutter::Actor.new
handle.background_color = Clutter::Color.new(:sky_blue)
handle.set_size(128, 128)
handle.set_position((800 - 128) / 2, (600 - 128) / 2)
handle.reactive = true
stage.add_child(handle)

handle.signal_connect("enter-event") do |actor, event|
  transition = actor.get_transition("curl")
  if transition.nil?
    transition = Clutter::PropertyTransition.new("@effects.curl.period")
    transition.duration = 250
    actor.add_transition("curl", transition);
  end

  transition.from = 0.0
  transition.to = 0.25
  transition.rewind
  transition.start

  Clutter::Event::STOP
end

handle.signal_connect("leave-event") do |actor, event|
  transition = actor.get_transition("curl")
  if transition.nil?
    transition = Clutter::PropertyTransition.new("@effects.curl.period")
    transition.duration = 250
    actor.add_transition("curl", transition)
  end

  transition.from = 0.25
  transition.to = 0.0
  transition.rewind
  transition.start

  Clutter::Event::STOP
end

action = Clutter::DragAction.new
action.set_drag_threshold(x_drag_threshold,
                          y_drag_threshold)
action.drag_axis = drag_axis

action.signal_connect("drag-begin") do |_action, actor, event_x, event_y, modifiers|
  copy_p = modifiers.shift_mask?
  if copy_p
    drag_handle = Clutter::Actor.new
    drag_handle.set_size(48, 48)
    drag_handle.background_color = Clutter::Color.new(:sky_blue_dark)
    stage.add_child(drag_handle)
    drag_handle.set_position(event_x, event_y)
  else
    drag_handle = actor
  end

  _action.drag_handle = drag_handle

  transition = actor.get_transition("disable")
  if transition.nil?
    transition = Clutter::PropertyTransition.new("@effects.disable.factor")
    transition.duration = 250
    actor.add_transition("disable", transition)
  end

  transition.from = 0.0
  transition.to = 1.0
  transition.rewind
  transition.start
end

action.signal_connect("drag-end") do |_action, actor, event_x, event_y, modifiers|
  drag_handle = _action.drag_handle
  if actor != drag_handle
    drag_handle.save_easing_state do
      drag_handle.easing_mode = :linear
      drag_handle.opacity = 0
    end
    drag_handle.signal_connect("transitions-completed") do
      drag_handle.destroy
    end

    parent = actor.parent
    succeeded, real_x, real_y = parent.transform_stage_point(event_x, event_y)

    actor.save_easing_state do
      actor.easing_mode = :ease_out_cubic
      actor.set_position(real_x, real_y)
    end
  end

  transition = actor.get_transition("disable")
  if transition.nil?
    transition = Clutter::PropertyTransition.new("@effects.disable.factor")
    transition.duration = 250
    actor.add_transition("disable", transition)
  end

  transition.from = 1.0
  transition.to = 0.0
  transition.rewind
  transition.start
end

handle.add_action(action)

handle.add_effect_with_name("disable", Clutter::DesaturateEffect.new(0.0))
handle.add_effect_with_name("curl", Clutter::PageTurnEffect.new(0.0, 45.0, 12.0))

stage.show

Clutter.main
