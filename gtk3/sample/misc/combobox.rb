#!/usr/bin/env ruby
=begin
  combobox.rb - Ruby/GTK sample script.

  Copyright (c) 2004-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

unless Gtk::Version.or_later?(3, 10, 7)
  puts "This sample requires GTK+ 3.10.7 or later"
  exit
end

window = Gtk::Window.new("Gtk::ComboBox sample")
window.signal_connect("destroy") {Gtk.main_quit}

#
# Text only
#
combo1 = Gtk::ComboBoxText.new
["foo", "bar", "fuga", "hoge"].each do |val|
  combo1.append_text(val)
end
combo1.active = 1

combo1.signal_connect("changed") do
  p "combo1: #{combo1.active}, #{combo1.active_iter[0]}"
end

#
# Icon and text
#
model = Gtk::ListStore.new(GdkPixbuf::Pixbuf, String)
[[Gtk::Stock::QUIT, "quit"],
 [Gtk::Stock::CANCEL, "cancel"],
 [Gtk::Stock::OK, "ok"]].each do |stock, name|
  iter = model.append
  iter[0] = window.render_icon_pixbuf(stock, Gtk::IconSize::MENU)
  iter[1] = name
end

combo2 = Gtk::ComboBox.new(:model => model)

# column 1
renderer = Gtk::CellRendererPixbuf.new
combo2.pack_start(renderer, false)
combo2.add_attribute(renderer, "pixbuf", 0)

# column 2
renderer = Gtk::CellRendererText.new
combo2.pack_start(renderer, true)
combo2.add_attribute(renderer, "text", 1)

combo2.active = 2

combo2.signal_connect("changed") do
  p "combo2: #{combo2.active}, #{combo2.active_iter[1]}"
end

#
# Gtk::ComboBoxEntry
#
combo3 = Gtk::ComboBoxText.new(:entry => true)
["foo", "bar", "fuga", "hoge"].each do |val|
  combo3.append_text(val)
end
combo3.active = 1

combo3.signal_connect("changed") do
  if combo3.active_iter
    p "combo3: #{combo3.active}, #{combo3.active_iter[0]}"
  end
end

# Show main window
vbox = Gtk::Box.new(:vertical)
vbox.add(combo1)
vbox.add(combo2)
vbox.add(combo3)
window.add(vbox)
window.show_all

Gtk.main
