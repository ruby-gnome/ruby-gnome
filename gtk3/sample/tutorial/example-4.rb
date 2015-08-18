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
#   * URL: https://git.gnome.org/browse/gtk+/tree/examples/builder.c
#   * License: LGPL 2

require "gtk3"

builder_file = "#{File.expand_path(File.dirname(__FILE__))}/builder.ui"

# Construct a Gtk::Builder instance and load our UI description
builder = Gtk::Builder.new(:file => builder_file)

# Connect signal handlers to the constructed widgets
window = builder.get_object("window")
window.signal_connect("destroy") { Gtk.main_quit }

button = builder.get_object("button1")
button.signal_connect("clicked") { puts "Hello World" }

button = builder.get_object("button2")
button.signal_connect("clicked") { puts "Hello World" }

button = builder.get_object("quit")
button.signal_connect("clicked") { Gtk.main_quit }

Gtk.main
