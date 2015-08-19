# Copyright (C) 2015  Ruby-GNOME2 Project Team
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
#
# Original:
#   * URL: https://git.gnome.org/browse/gtk+/tree/examples/grid-packing.c
#   * License: LGPL 2

require "gtk3"

app = Gtk::Application.new("org.gtk.example", :flags_none)

app.signal_connect "activate" do |application|
  # create a new window, and set its title
  window = Gtk::ApplicationWindow.new(application)
  window.set_title("Window")
  window.set_border_width(10)

  # Here we construct the container that is going pack our buttons 
  grid = Gtk::Grid.new

  # Pack the container in the window
  window.add(grid)

  button = Gtk::Button.new(:label => "Button 1")
  button.signal_connect("clicked") { puts "Hello World" }
  # Place the first button in the grid cell (0, 0), and make it fill
  # just 1 cell horizontally and vertically (ie no spanning)
  grid.attach(button, 0, 0, 1, 1)

  button = Gtk::Button.new(:label => "Button 2")
  button.signal_connect("clicked") { puts "Hello World" }
  # Place the second button in the grid cell (1, 0), and make it fill
  # just 1 cell horizontally and vertically (ie no spanning)
  grid.attach(button, 1, 0, 1, 1)

  button = Gtk::Button.new(:label => "Quit")
  button.signal_connect("clicked") { window.destroy }
  # Place the Quit button in the grid cell (0, 1), and make it
  # span 2 columns.
  grid.attach(button, 0, 1, 2, 1)

  # Now that we are done packing our widgets, we show them all
  # in one go, by calling Gtk::Widget#show_all on the window.
  # This call recursively calls Gtk::Widget#show on all widgets
  # that are contained in the window, directly or indirectly
  window.show_all
end

# Gtk::Application#run need C style argv ([prog, arg1, arg2, ...,argn]).
# The ARGV ruby variable only contains the arguments ([arg1, arg2, ...,argb])
# and not the program name. We have to add it explicitly.

status = app.run([$0] + ARGV)

puts status
