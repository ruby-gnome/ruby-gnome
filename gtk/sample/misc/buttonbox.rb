#!/usr/local/bin/ruby
=begin
  buttonbox.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: buttonbox.rb,v 1.6 2003/04/01 16:26:32 mutoh Exp $
=end

require 'gtk2'

def create_bbox_window(horizontal, title, pos, spacing, layout)
  window = Gtk::Window.new
  window.set_title(title)
  window.signal_connect("delete_event") do
    window.hide
    window.destroy
  end
  if horizontal
    window.set_default_size(550, 60)
    window.move(150, pos)
  else
    window.set_default_size(150, 400)
    window.move(pos, 200)
  end
  box1 = Gtk::VBox.new(false, 0)
  window.add box1
  if horizontal
    bbox = Gtk::HButtonBox.new
  else
    bbox = Gtk::VButtonBox.new
  end
  bbox.set_layout_style layout
  bbox.set_spacing spacing
  box1.set_border_width 25
  box1.pack_start(bbox, true, true, 0)
  button = Gtk::Button.new("OK")
  bbox.add button
  button.signal_connect("clicked") do
    window.hide
    window.destroy
  end

  button = Gtk::Button.new("Cancel")
  bbox.add button
  button.signal_connect("clicked") do
    window.hide
    window.destroy
  end

  button = Gtk::Button.new("Help")
  bbox.add button
  window.show_all
end

def test_hbbox
  create_bbox_window(true, "Spread", 50, 40, Gtk::ButtonBox::SPREAD)
  create_bbox_window(true, "Edge", 250, 40, Gtk::ButtonBox::EDGE)
  create_bbox_window(true, "Start", 450, 40,  Gtk::ButtonBox::START)
  create_bbox_window(true, "End", 650, 15, Gtk::ButtonBox::END)
end

def test_vbbox
  create_bbox_window(false, "Spread", 50, 40, Gtk::ButtonBox::SPREAD)
  create_bbox_window(false, "Edge", 250, 40,  Gtk::ButtonBox::EDGE)
  create_bbox_window(false, "Start", 450, 40, Gtk::ButtonBox::START)
  create_bbox_window(false, "End", 650, 15, Gtk::ButtonBox::END)
end

Gtk.init

window = Gtk::Window.new
window.signal_connect("delete_event") do
	Gtk.main_quit
end
window.set_title("button box")
window.set_border_width(20)

bbox = Gtk::HButtonBox.new
window.add(bbox)

button = Gtk::Button.new("Horizontal")
def button.clicked(*args)
  test_hbbox
end
bbox.add button

button = Gtk::Button.new("Vertical")
def button.clicked(*args)
  test_vbbox
end
bbox.add button

window.show_all

Gtk.main
