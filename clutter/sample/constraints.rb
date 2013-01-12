#!/usr/bin/env ruby
#
# This sample code is a port of clutter/examples/constraints.c.
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

stage = Clutter::Stage.new
stage.name = "stage"
stage.title = "Snap Constraint"
stage.user_resizable = true
stage.background_color = Clutter::Color.get_static(:aluminium_1)
stage.signal_connect("destroy") do
  Clutter.main_quit
end

layer_a = Clutter::Actor.new
layer_a.background_color = Clutter::Color.new(:scarlet_red)
layer_a.name = "layerA"
layer_a.set_size(100.0, 25.0)
stage.add_child(layer_a)

layer_a.add_constraint(Clutter::AlignConstraint.new(stage, :both, 0.5))

layer_b = Clutter::Actor.new
layer_b.background_color = Clutter::Color.new(:butter_dark)
layer_b.name = "layerB"
stage.add_child(layer_b)

layer_b.add_constraint(Clutter::BindConstraint.new(layer_a, :x, 0.0))
layer_b.add_constraint(Clutter::BindConstraint.new(layer_a, :width, 0.0))

layer_b.add_constraint(Clutter::SnapConstraint.new(layer_a,
                                                   :top,
                                                   :bottom,
                                                   10.0))

layer_b.add_constraint(Clutter::SnapConstraint.new(stage,
                                                   :bottom,
                                                   :bottom,
                                                   -10.0))

layer_c = Clutter::Actor.new
layer_c.background_color = Clutter::Color.new(:chameleon_light)
layer_c.name = "layerC"
stage.add_child(layer_c)

layer_c.add_constraint(Clutter::BindConstraint.new(layer_a, :x, 0.0))
layer_c.add_constraint(Clutter::BindConstraint.new(layer_a, :width, 0.0))

layer_c.add_constraint(Clutter::SnapConstraint.new(layer_a,
                                                   :bottom,
                                                   :top,
                                                   -10.0))
layer_c.add_constraint(Clutter::SnapConstraint.new(stage,
                                                   :top,
                                                   :top,
                                                   10.0))

stage.show

Clutter.main
