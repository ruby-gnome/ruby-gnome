#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/image-content.c. The
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

Clutter.init

gravities = Clutter::ContentGravity.values
current_gravity_index = gravities.size - 1

stage = Clutter::Stage.new
stage.name = "Stage"
stage.title = "Content Box"
stage.user_resizable = true

stage.signal_connect("destroy") do
  Clutter.main_quit
end

stage.show

box = Clutter::Actor.new
box.name = "Image"
box.margin_top = 12
box.margin_right = 12
box.margin_bottom = 12
box.margin_left = 12
box.add_constraint(Clutter::BindConstraint.new(stage, :size, 0.0))
stage.add_child(box)

pixbuf = Gdk::Pixbuf.new(File.expand_path("redhand.png", File.dirname(__FILE__)))
image = Clutter::Image.new
image.set_data(pixbuf.pixels,
               pixbuf.has_alpha? ? :rgba_8888 : :rgb_888,
               pixbuf.width,
               pixbuf.height,
               pixbuf.rowstride)

box.set_content_scaling_filters(:trilinear, :linear)
box.content_gravity = gravities[current_gravity_index]
box.content = image

text = Clutter::Text.new
text.add_constraint(Clutter::AlignConstraint.new(stage, :both, 0.5))
stage.add_child(text)

update_text = lambda do
  current_gravity = gravities[current_gravity_index]
  text.text = "Content gravity: #{current_gravity.nick}"
end
update_text.call

action = Clutter::TapAction.new
action.signal_connect("tap") do |_action, actor|
  current_gravity_index = (current_gravity_index + 1) % gravities.size
  actor.save_easing_state do
    actor.content_gravity = gravities[current_gravity_index]
  end

  update_text.call
end

box.reactive = true
box.add_action(action)

Clutter.main
