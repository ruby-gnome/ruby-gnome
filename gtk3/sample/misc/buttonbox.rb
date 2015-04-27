#!/usr/bin/env ruby
=begin
  buttonbox.rb - Ruby/GTK3 sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

def create_bbox_window(horizontal, title, pos, spacing, layout)
  window = Gtk::Window.new(title)
  window.signal_connect("delete_event") do
    window.destroy
  end

  box1 = Gtk::Box.new(:vertical, 0)
  window.add(box1)

  if horizontal
    window.set_default_size(550, 60)
    window.move(150, pos)
    bbox = Gtk::ButtonBox.new(:horizontal)
  else
    window.set_default_size(150, 400)
    window.move(pos, 200)
    bbox = Gtk::ButtonBox.new(:vertical)
  end

  bbox.layout_style = layout
  bbox.spacing = spacing
  box1.border_width = 25
  box1.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

  button = Gtk::Button.new(:label => "OK")
  bbox.add(button)
  button.signal_connect("clicked") do
    window.destroy
  end

  button = Gtk::Button.new(:label => "Cancel")
  bbox.add(button)
  button.signal_connect("clicked") do
    window.destroy
  end

  button = Gtk::Button.new(:label => "Help")
  bbox.add(button)
  window.show_all
end

def test_hbbox
  create_bbox_window(true, "Spread", 50, 40, :spread)
  create_bbox_window(true, "Edge", 250, 40, :edge)
  create_bbox_window(true, "Start", 450, 40,  :start)
  create_bbox_window(true, "End", 650, 15, :end)
end

def test_vbbox
  create_bbox_window(false, "Spread", 50, 40, :spread)
  create_bbox_window(false, "Edge", 250, 40,  :edge)
  create_bbox_window(false, "Start", 450, 40, :start)
  create_bbox_window(false, "End", 650, 15, :end)
end

window = Gtk::Window.new("button box")
window.signal_connect("delete_event") do
  Gtk.main_quit
end
window.border_width = 20

bbox = Gtk::ButtonBox.new(:horizontal)
window.add(bbox)

button = Gtk::Button.new(:label => "Horizontal")

button.signal_connect("clicked") do
  test_hbbox
end

bbox.add(button)

button = Gtk::Button.new(:label => "Vertical")
button.signal_connect("clicked") do
  test_vbbox
end
bbox.add(button)

window.show_all

Gtk.main
