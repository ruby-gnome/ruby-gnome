#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/pan-action.c. The
# image file used in this sample code is copied from
# clutter/tests/data/redhand.png. They are licensed under the terms
# of the GNU Lesser General Public License, version 2.1 or (at your
# option) later.
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
require "gdk_pixbuf2"

stage = Clutter::Stage.new
stage.name = "Pan Action"
stage.user_resizable = true

scroll = Clutter::Actor.new
scroll.name = "scroll"

scroll.add_constraint(Clutter::AlignConstraint.new(stage, :x_axis, 0))
scroll.add_constraint(Clutter::BindConstraint.new(stage, :size, 0))

content = Clutter::Actor.new
content.set_size(720, 720)

pixbuf = Gdk::Pixbuf.new(File.expand_path("redhand.png", File.dirname(__FILE__)))
image = Clutter::Image.new
image.set_data(pixbuf.pixels,
               pixbuf.has_alpha? ? :rgba_8888 : :rgb_888,
               pixbuf.width,
               pixbuf.height,
               pixbuf.rowstride)

content.set_content_scaling_filters(:trilinear, :linear)
content.content_gravity = :resize_aspect
content.content = image

scroll.add_child(content)

pan_action = Clutter::PanAction.new
pan_action.interpolate = true
pan_action.signal_connect("pan") do |action, actor, interpolated_p|
  if interpolated_p
    delta_x, delta_y = action.interpolated_delta
    event_name = "INTERPOLATED"
  else
    delta_x, delta_y = action.get_motion_delta(0)
    event = action.get_last_event(0)
    event_name = event.class.name
  end

  puts("[#{event_name}] panning dx:#{delta_x} dy:#{delta_y}");
  Clutter::Event::STOP
end

scroll.add_action(pan_action)
scroll.reactive = true

stage.add_child(scroll)

info = Clutter::Text.new(nil, "Press <space> to reset the image position.")
stage.add_child(info)
info.set_position(12, 12)

stage.signal_connect("destroy") do
  Clutter.main_quit
end

stage.signal_connect("key-press-event") do |actor, event|
  case event.key_symbol
  when Clutter::Keys::KEY_space
    scroll.save_easing_state do
      scroll.easing_duration = 1000
      scroll.child_transform = nil
    end
  end

  Clutter::Event::STOP
end

stage.show

Clutter.main
