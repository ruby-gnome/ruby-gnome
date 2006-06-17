#!/usr/bin/env ruby
=begin
  combobox.rb - Ruby/GTK sample script.

  Copyright (c) 2004-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: combobox.rb,v 1.3 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

if str = Gtk.check_version(2, 4, 0)
  puts "This sample requires GTK+ 2.4.0 or later"
  puts str
  exit
end

window = Gtk::Window.new("Gtk::ComboBox sample")
window.signal_connect("destroy"){Gtk.main_quit}

#
# Text only
#
combo1 = Gtk::ComboBox.new
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
model = Gtk::ListStore.new(Gdk::Pixbuf, String)
[[Gtk::Stock::QUIT, "quit"],
 [Gtk::Stock::CANCEL, "cancel"],
 [Gtk::Stock::OK, "ok"]].each do |stock, name|
  iter = model.append
  iter[0] = window.render_icon(stock, Gtk::IconSize::MENU, "icon")
  iter[1] = name
end

combo2 = Gtk::ComboBox.new(model)

# column 1
renderer = Gtk::CellRendererPixbuf.new
combo2.pack_start(renderer, false)
combo2.set_attributes(renderer, :pixbuf => 0)

# column 2
renderer = Gtk::CellRendererText.new
combo2.pack_start(renderer, true)
combo2.set_attributes(renderer, :text => 1)

combo2.active = 2

combo2.signal_connect("changed") do
  p "combo2: #{combo2.active}, #{combo2.active_iter[1]}"
end

#
# Gtk::ComboBoxEntry
#
combo3 = Gtk::ComboBoxEntry.new
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
vbox = Gtk::VBox.new
vbox.add(combo1).add(combo2).add(combo3)
window.add(vbox).show_all

Gtk.main

