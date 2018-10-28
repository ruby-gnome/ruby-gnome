#!/usr/bin/env ruby
#
# Copyright (C) 2018  Ruby-GNOME2 Project Team
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

require "gegl"

if ARGV.size != 2
  puts("#{$0} FROM TO")
  exit(false)
end

from = ARGV[0]
to = ARGV[1]

graph = Gegl::Node.new
load = graph.create_child("gegl:load")
load.set_property("path", from)
save = graph.create_child("gegl:save")
save.set_property("path", to)
load << save
save.process
