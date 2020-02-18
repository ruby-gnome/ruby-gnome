#!/usr/bin/env ruby
=begin
  scalebutton.rb - Very simple Gtk::ScaleButton example.

  Copyright (c) 2007-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "gtk3"

window = Gtk::Window.new
window.set_size_request(300, 300)
scale = Gtk::ScaleButton.new
scale.set_value(30)
scale.set_valign(:end)
scale.set_halign(:end)
box = Gtk::Box.new(:vertical)
box.add(scale)
box.vexpand = true
box.hexpand = true

window.add(box)
# see here for an explication  on icons order
# https://developer.gnome.org/gtk3/stable/GtkScaleButton.html#GtkScaleButton--icons
scale.set_icons(%w(go-bottom go-top system-run))
scale.signal_connect("value-changed") { |_widget, value| puts "value changed: #{value}" }

window.show_all

window.signal_connect("destroy") { Gtk.main_quit }
Gtk.main
