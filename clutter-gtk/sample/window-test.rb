#!/usr/bin/env ruby
#
# This sample code is a port of clutter-gtk/examples/gtk-clutter-window-test.c.
# It is licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
#
# The original header:
#   (c) 2009, Collabora Ltd.
#
#   Written by Davyd Madeley <davyd.madeley@collabora.co.uk>
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

ClutterGtk.init

window = ClutterGtk::Window.new
window.signal_connect("destroy") do
  Gtk.main_quit
end
window.set_default_size(400, 300)

NAME_COLUMN   = 0
PIXBUF_COLUMN = 1
store = Gtk::ListStore.new(String, Gdk::Pixbuf)
theme = Gtk::IconTheme.default
[
  "devhelp",
  "empathy",
  "evince",
  "gnome-panel",
  "seahorse",
  "sound-juicer",
  "totem",
].each do |icon_name|
  pixbuf = theme.load_icon(icon_name, 48, 0)
  iter = store.append
  iter[0] = icon_name
  iter[1] = pixbuf
end

icon_view = Gtk::IconView.new(store)
icon_view.text_column = NAME_COLUMN
icon_view.pixbuf_column = PIXBUF_COLUMN

scrolled_window = Gtk::ScrolledWindow.new
window.add(scrolled_window)
scrolled_window.add(icon_view)
scrolled_window.show_all

# Widget 2 is a toolbar
stage = window.stage

toolbar = Gtk::Toolbar.new
[
  Gtk::Stock::ADD,
  Gtk::Stock::BOLD,
  Gtk::Stock::ITALIC,
  Gtk::Stock::CANCEL,
  Gtk::Stock::CDROM,
  Gtk::Stock::CONVERT,
].each do |stock_id|
  item = Gtk::ToolButton.new(:stock_id => stock_id)
  toolbar.insert(item, -1)
end

toolbar.show_all
actor = ClutterGtk::Actor.new(toolbar)
actor.add_constraint(Clutter::BindConstraint.new(stage, :width, 0.0))
actor.signal_connect("enter-event") do |_actor, event|
  _actor.save_easing_state do
    _actor.easing_mode = :linear

    _actor.opacity = 255
    _actor.y = 0
  end

  Clutter::Event::STOP
end

actor.signal_connect("leave-event") do |_actor, event|
  _actor.save_easing_state do
    _actor.easing_mode = :linear

    _actor.opacity = 128
    _actor.y = _actor.height * -0.5
  end

  Clutter::Event::STOP
end

actor.y = actor.height * -0.5
actor.opacity = 128
actor.reactive = true
stage.add_child(actor)

window.show_all

Gtk.main
