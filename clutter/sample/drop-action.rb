#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/drop-action.c.
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

require "clutter"

Clutter.init

TARGET_SIZE = 200
HANDLE_SIZE = 128

stage = Clutter::Stage.new
stage.title = "Drop Action"
stage.signal_connect("destroy") do
  Clutter.main_quit
end

target1 = Clutter::Actor.new
target1.background_color = Clutter::Color.new(:scarlet_red_light)
target1.set_size(TARGET_SIZE, TARGET_SIZE)
target1.opacity = 64
target1.add_constraint(Clutter::AlignConstraint.new(stage, :y_axis, 0.5))
target1.x = 10
target1.reactive = true

on_target_over = lambda do |action, actor, over_p|
  final_opacity = over_p ? 128 : 64

  target = action.actor
  target.save_easing_state do
    target.easing_mode = :linear
    target.opacity = final_opacity
  end
end

target1.add_action_with_name("drop", Clutter::DropAction.new)
drop_action = target1.get_action("drop")
drop_action.signal_connect("over-in") do |_action, actor|
  on_target_over.call(_action, actor, true)
end
drop_action.signal_connect("over-out") do |_action, actor|
  on_target_over.call(_action, actor, false)
end

drag = nil
drop_successful = false
add_drag_object = lambda do |target|
  if drag.nil?
    drag = Clutter::Actor.new
    drag.background_color = Clutter::Color.new(:sky_blue_light)
    drag.set_size(HANDLE_SIZE, HANDLE_SIZE)
    drag.set_position((TARGET_SIZE - HANDLE_SIZE) / 2.0,
                      (TARGET_SIZE - HANDLE_SIZE) / 2.0)
    drag.reactive = true

    action = Clutter::DragAction.new
    action.signal_connect("drag-begin") do |_action, _actor, event_x, event_y, modifiers|
      position = _actor.position
      handle = Clutter::Actor.new
      handle.background_color = Clutter::Color.new(:sky_blue_dark)
      handle.set_size(128, 128)
      handle.set_position(event_x - position.x, event_y - position.y)
      stage.add_child(handle)

      _action.drag_handle = handle

      _actor.save_easing_state do
        _actor.easing_mode = :linear
        _actor.opacity = 128
      end

      drop_successful = true
    end
    action.signal_connect("drag-end") do |_action, _actor, event_x, event_y, modifiers|
      handle = _action.drag_handle
      printf("Drag ended at: %.0f, %.0f\n", event_x, event_y)
      _actor.save_easing_state do
        _actor.easing_mode = :linear
        _actor.opacity = 255
      end

      handle.save_easing_state do
        if drop_successful
          handle.easing_mode = :linear
          handle.opacity = 0
        else
          parent = _actor.parent

          parent.save_easing_state do
            parent.easing_mode = :linear
            parent.opacity = 255
          end

          x_pos, y_pos = _actor.transformed_position

          handle.easing_mode = :ease_out_bounce
          handle.set_position(x_pos, y_pos)
          handle.opacity = 0
        end
      end

      handle.signal_connect("transitions-completed") do |_actor|
        _actor.destroy
      end
    end

    drag.add_action(action)
  end

  parent = drag.parent
  if parent == target
    target.save_easing_state do
      target.easing_mode = :linear
      target.opacity = 255
    end
    return
  end

  if parent and parent != stage
    parent.remove_child(drag)

    parent.save_easing_state do
      parent.easing_mode :linear
      parent.opacity = 64
    end
  end
  target.add_child(drag)

  target.save_easing_state do
    target.easing_mode :linear
    target.opacity = 255
  end
end

on_target_drop = lambda do |_action, actor, event_x, event_y|
  succeeded, actor_x, actor_y = actor.transform_stage_point(event_x, event_y)
  printf("Dropped at %.0f, %.0f (screen: %.0f, %.0f)\n",
         actor_x, actor_y,
         event_x, event_y)

  drop_successful = TRUE;
  add_drag_object.call(actor)
end
drop_action.signal_connect("drop", &on_target_drop)

dummy = Clutter::Actor.new
dummy.background_color = Clutter::Color.new(:orange_dark)
dummy.set_size(640 - (2 * 10) - (2 * (TARGET_SIZE + 10)),
               TARGET_SIZE)
dummy.add_constraint(Clutter::AlignConstraint.new(stage, :x_axis, 0.5))
dummy.add_constraint(Clutter::AlignConstraint.new(stage, :y_axis, 0.5))
dummy.reactive = true

target2 = Clutter::Actor.new
target2.background_color = Clutter::Color.new(:chameleon_light)
target2.set_size(TARGET_SIZE, TARGET_SIZE)
target2.opacity = 64
target2.add_constraint(Clutter::AlignConstraint.new(stage, :y_axis, 0.5))
target2.x = 640 - TARGET_SIZE - 10
target2.reactive = true

target2.add_action_with_name("drop", Clutter::DropAction.new)
drop_action = target2.get_action("drop")
drop_action.signal_connect("over-in") do |_action, actor|
  on_target_over.call(_action, actor, true)
end
drop_action.signal_connect("over-out") do |_action, actor|
  on_target_over.call(_action, actor, false)
end
drop_action.signal_connect("drop", &on_target_drop)

stage.add_child(target1)
stage.add_child(dummy)
stage.add_child(target2)

add_drag_object.call(target1)

stage.show

Clutter.main
