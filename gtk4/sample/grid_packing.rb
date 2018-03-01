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

# example from https://github.com/GNOME/gtk/blob/master/examples/grid-packing.c

require_relative "utils"

require_gtk4

application = Gtk::Application.new("org.gtk.example", :flags_none)

application.signal_connect "activate" do |app|
  # Create a new window and set its title
  win = Gtk::ApplicationWindow.new(app)
  win.title = "window"

  # Here we construct the container that is going to pack our buttons.
  grid = Gtk::Grid.new

  # Pack the container in the window
  win.add(grid)

  button = Gtk::Button.new(:label => "Button 1")
  button.signal_connect("clicked") { puts "Hello World!" }

  # Place the first button in the grid cell (0, 0), and make it fill
  # just 1 cell horizontally and vertically (ie no spanning)
  grid.attach(button, 0, 0, 1, 1)

  button = Gtk::Button.new(:label => "Button 2")
  button.signal_connect("clicked") { puts "Hello World!" }

  # Place the second button in the grid cell (1, 0), and make it fill
  # just 1 cell horizontally and vertically (ie no spanning)
  grid.attach(button, 1, 0, 1, 1)

  button = Gtk::Button.new(:label => "Quit")
  button.signal_connect("clicked") { win.destroy }

  # Place the Quit button in the grid cell (0, 1), and make it
  # span 2 columns.
  grid.attach(button, 0, 1, 2, 1)

  win.show
end

application.run
