#!/usr/bin/env ruby
#
# Copyright (C) 2013-2022  Ruby-GNOME Project Team
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

# example from https://gitlab.gnome.org/GNOME/gtk/blob/main/examples/hello/hello-world.c

require_relative "utils"

require_gtk4

application = Gtk::Application.new("org.gtk.example", :flags_none)

application.signal_connect "activate" do |app|
  window = Gtk::ApplicationWindow.new(app)
  window.title = "Window"
  window.set_default_size(200, 200)

  button = Gtk::Button.new(label: "Hello World")
  button.halign = :center
  button.valign = :center

  button.signal_connect "clicked" do
    puts "Hello World"
    window.destroy
  end

  window.child = button

  window.show
end

application.run
