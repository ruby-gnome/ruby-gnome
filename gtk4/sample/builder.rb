#!/usr/bin/env ruby
#
# Copyright (C) 2013-2018  Ruby-GNOME2 Project Team
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

# example from https://github.com/GNOME/gtk/blob/master/examples/builder.c

require_relative "utils"

require_gtk4

# Construct a GtkBuilder instance and load our UI description
builder = Gtk::Builder.new(:file => "#{path}/builder.ui")

# Connect signal handlers to the constructed widgets.
window = builder["window"]
window.signal_connect("destroy") { Gtk.main_quit }

button = builder["button1"]
button.signal_connect("clicked") { puts "Hello World!" }

button = builder["button2"]
button.signal_connect("clicked") { puts "Hello World!" }

button = builder["quit"]
button.signal_connect("clicked") { Gtk.main_quit }

Gtk.main
