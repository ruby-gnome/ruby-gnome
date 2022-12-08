#!/usr/bin/env ruby
#
# Copyright (C) 2022  Ruby-GNOME Project Team
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
# Code: https://gitlab.gnome.org/GNOME/libhandy/-/blob/main/examples/example.py
# License: LGPL-2.1+
# Author: https://gitlab.gnome.org/GNOME/libhandy/-/blob/main/AUTHORS

require "handy"

window = Gtk::Window.new("Keypad Example with Ruby")
vbox = Gtk::Box.new(:vertical)
entry = Gtk::Entry.new
keypad = Handy::Keypad.new(true, true)

vbox.add(entry)     # widget to show dialed number
vbox.add(keypad)
vbox.set_halign(:center)
vbox.set_valign(:center)

vbox.margin = 18
vbox.spacing = 18
keypad.set_row_spacing(6)
keypad.set_column_spacing(6)

keypad.set_entry(entry)     # attach the entry widget

window.signal_connect("destroy") do
  Gtk.main_quit
end
window.add(vbox)
window.show_all
Gtk.main
