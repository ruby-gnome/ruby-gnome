#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/scroll-actor.c.
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

stage = Clutter::Stage.new
stage.title = "Scroll Actor"
stage.user_resizable = true

stage.signal_connect("destroy") do
  Clutter.main_quit
end

selected_item_index = nil
select_item_at_index = lambda do |scroll, index|
  menu = scroll.first_child
  if selected_item_index
    item = menu.get_child_at_index(selected_item_index)
    item.color = Clutter::Color.new(:white)
  end

  index = index % menu.n_children

  item = menu.get_child_at_index(index)
  succeeded, x, y = item.position

  scroll.save_easing_state do
    scroll.scroll_to_point(Clutter::Point.new(x, y))
  end

  item.color = Clutter::Color.new(:sky_blue_light)
  selected_item_index = index
end

stage.signal_connect("key-press-event") do |actor, event|
  scroll = stage.first_child

  case event.key_symbol
  when Clutter::Keys::KEY_Up
    select_item_at_index.call(scroll, selected_item_index - 1)
  when Clutter::Keys::KEY_Down
    select_item_at_index.call(scroll, selected_item_index + 1)
  end

  Clutter::Event::STOP
end

scroll = Clutter::ScrollActor.new
scroll.name = "scroll"

scroll.set_position(0.0, 18.0)
scroll.add_constraint(Clutter::AlignConstraint.new(stage, :x_axis, 0.5))
scroll.add_constraint(Clutter::BindConstraint.new(stage, :height, -36.0))

scroll.scroll_mode = :vertically

layout_manager = Clutter::BoxLayout.new
layout_manager.orientation = :vertical
layout_manager.spacing = 12.0

menu = Clutter::Actor.new
menu.layout_manager = layout_manager
menu.background_color = Clutter::Color.new(:black)

n_items = 11
n_items.times do |i|
  text = Clutter::Text.new
  text.font_name = "Sans Bold 24"
  text.text = "Option #{i + 1}"
  text.color = Clutter::Color.new(:white)
  text.margin_left = 12.0
  text.margin_right = 12.0
  menu.add_child(text)
end

scroll.add_child(menu)

select_item_at_index.call(scroll, 0)

stage.add_child(scroll)

stage.show

Clutter.main
