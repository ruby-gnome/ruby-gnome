#!/usr/bin/env ruby
=begin
  misc_button.rb - Ruby/GTK sample script.

  Copyright (c) 2004-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "gtk3"

box = Gtk::Box.new(:vertical)

# Gtk::ColorButton
colorbutton = Gtk::ColorButton.new
colorbutton.signal_connect("color-set") do
  p colorbutton.color.to_s
end
box.add(colorbutton)

# Gtk::FontButton
fontbutton = Gtk::FontButton.new
fontbutton.signal_connect("font-set") do
  p fontbutton.font_name
end
box.add(fontbutton)

# Gtk::FileChooserButton
filebutton = Gtk::FileChooserButton.new("Gtk::FileChooserButton", :open)
filebutton.filename = GLib.home_dir
filebutton.signal_connect("current-folder-changed") do |w, e|
  p filebutton.filename
end

box.add(filebutton)

# Quit
quitbutton = Gtk::Button.new(:label => "Quit")
quitbutton.signal_connect("clicked") do
  Gtk.main_quit
end
box.add(quitbutton)

win = Gtk::Window.new
win.add(box)
win.set_default_size(200, 100)
win.show_all
win.signal_connect("delete-event") { Gtk.main_quit }

Gtk.main
