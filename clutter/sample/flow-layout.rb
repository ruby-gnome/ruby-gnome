#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/flow-layout.c.
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

random_size_p = false
n_rects       = 20
vertical_p    = false
homogeneous_p = false
x_spacing     = 0
y_spacing     = 0
fixed_size_p  = false

parser = OptionParser.new
parser.on("--[no-]random-size",
          "Randomly size the rectangles",
          "(#{random_size_p})") do |boolean|
  random_size_p = boolean
end
parser.on("--n-rects=N", Integer,
          "Number of rectangles",
          "(#{n_rects})") do |n|
  n_rects = n
end
parser.on("--[no-]vertical",
          "Set vertical orientation",
          "(#{vertical_p})") do |boolean|
  vertical_p = boolean
end
parser.on("--[no-]homogeneous",
          "Whether the layout should be homogeneous",
          "(#{homogeneous_p})") do |boolean|
  homogeneous_p = boolean
end
parser.on("--x-spacing=PIXEL", Integer,
          "Horizontal spacing between elements",
          "(#{x_spacing})") do |pixel|
  x_spacing = pixel
end
parser.on("--y-spacing=PIXEL", Integer,
          "Vertical spacing between elements",
          "(#{y_spacing})") do |pixel|
  y_spacing = pixel
end
parser.on("--[no-]fixed-size",
          "Fix the layout size",
          "(#{fixed_size_p})") do |boolean|
  fixed_size_p = boolean
end
parser.parse!

stage = Clutter::Stage.new
stage.background_color = Clutter::Color.new(:sky_blue_light)
stage.title = "Flow Layout"
stage.user_resizable = true
stage.signal_connect("destroy") do
  Clutter.main_quit
end

layout = Clutter::FlowLayout.new(vertical_p ? :vertical : :horizontal)
layout.homogeneous = homogeneous_p
layout.column_spacing = x_spacing
layout.row_spacing = y_spacing

box = Clutter::Actor.new
box.layout_manager = layout
box.background_color = Clutter::Color.new(:aluminium_2)
stage.add_child(box)

unless fixed_size_p
  box.add_constraint(Clutter::BindConstraint.new(stage, :size, 0.0))
end

box.set_position(0, 0)
box.name = "box"

n_rects.times do |i|
  color = Clutter::Color.hls(360.0 / n_rects * i,
                             0.5,
                             0.8)
  rect = Clutter::Actor.new
  rect.background_color = color
  if random_size_p
    width = rand(50..100)
    height = rand(50..100)
  else
    width = height = 50.0
  end

  rect.set_size(width, height)
  rect.name = "rect%02d" % i

  box.add_child(rect)
end

stage.show

Clutter.main
