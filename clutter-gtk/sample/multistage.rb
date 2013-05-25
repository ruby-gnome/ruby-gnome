#!/usr/bin/env ruby
#
# This sample code is a port of clutter-gtk/examples/gtk-clutter-multistage.c.
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

require "clutter-gtk"
require "gdk_pixbuf2"

window = Gtk::Window.new
window.set_default_size(600, 400)
window.title = "Multiple GtkClutterEmbed"
window.signal_connect("destroy") do
  Gtk.main_quit
end

notebook = Gtk::Notebook.new
window.add(notebook)

clutter0 = ClutterGtk::Embed.new
notebook.append_page(clutter0, Gtk::Label.new("One stage"))
stage0 = clutter0.stage
stage0.background_color = Clutter::Color.new(0xdd, 0xff, 0xdd, 0xff)

vbox = Gtk::Box.new(:vertical, 6)
notebook.append_page(vbox, Gtk::Label.new("Two stages"))

clutter1 = ClutterGtk::Embed.new
clutter1.set_size_request(320, 240)
stage1 = clutter1.stage
stage1.background_color = Clutter::Color.new(0xff, 0xff, 0xff, 0xff)
texture1 = ClutterGtk::Texture.new
texture1.set_from_stock(clutter1, Gtk::Stock::DIALOG_INFO.to_s, :dialog)
texture1.set_anchor_point(texture1.width / 2,
                          texture1.height / 2)
texture1.set_position(160, 120)
stage1.add_child(texture1)
texture1.show

vbox.add(clutter1)

clutter2 = ClutterGtk::Embed.new
clutter2.set_size_request(320, 120)
stage2 = clutter2.stage
stage2.background_color = Clutter::Color.new(0x00, 0x00, 0x00, 0xff)
texture2 = ClutterGtk::Texture.new
texture2.set_from_icon_name(clutter1, "user-info", :button)
texture2.set_anchor_point(texture2.width / 2,
                          texture2.height / 2)
texture2.set_position(160, 60)
stage2.add_child(texture2)

vbox.add(clutter2)

stage2.signal_connect("allocation-changed") do |actor, allocation, flags|
  texture2.set_position((allocation.x2 - allocation.x1) / 2,
                        (allocation.y2 - allocation.y1) / 2)
end

window.show_all

Gtk.main
