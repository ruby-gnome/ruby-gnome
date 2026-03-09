#!/usr/bin/env ruby
#
# Copyright (C) 2024  Ruby-GNOME Project Team
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
# Example from:
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application1/exampleapp.c
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application1/exampleappwin.c
# License: LGPL2.1-or-later


require "gtk4"

def on_activate(app)
  win = Gtk::ApplicationWindow.new(app)
  win.title = "Your Interests"
  win.set_default_size(350, 350)
  win.modal = true  
  rbox = Gtk::Box.new(:vertical)
  revealer = Gtk::Revealer.new()
  button = Gtk::Button.new(label: "Click to Reveal")
  button.signal_connect("clicked") do |*a|
    revealer.reveal_child = !revealer.child_revealed? #toggle
  end
  rbox.append(button)
  list_box = Gtk::ListBox.new()
  list_box.margin_start = 20
  list_box.margin_end = 20
  list_box.margin_top = 20
  list_box.append(box("Politics and Culture", false))
  list_box.append(box("Outdoors", true)) 
  list_box.append(box("Movies and Plays", false))
  list_box.append(box("Recreation and Sports", true))
  list_box.append(box("Working", true)) 
  revealer.set_child(list_box) 
  rbox.append(revealer)  
  win.set_child(rbox)
  win.present()
end

def box(text, on_off)
  box = Gtk::Box.new(:horizontal)
  label = Gtk::Label.new(text)
  box.append(Gtk::Label.new(text))
  switch = Gtk::Switch.new()
  switch.halign = :end
  switch.hexpand = true
  switch.active = on_off
  box.append(switch)
  return box
end


app = Gtk::Application.new("io.github.ruby-gnome.gtk4.example.Revealer")
app.signal_connect("activate") do |a|
  on_activate(a)
end
app.run
