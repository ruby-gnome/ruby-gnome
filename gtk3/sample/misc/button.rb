#!/usr/bin/env ruby
=begin
  button.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: button.rb,v 1.10 2006/06/17 13:18:12 mutoh Exp $
=end

require "gtk3"

window = Gtk::Window.new("buttons")
window.border_width = 0
window.signal_connect("delete_event") do
  Gtk.main_quit
end

box1 = Gtk::Box.new(:vertical, 0)
window.add(box1)

table = Gtk::Table.new(3, 3, false)
table.set_row_spacings(5)
table.set_column_spacings(5)
table.set_border_width(10)
box1.pack_start(table, :expand => true, :fill => true, :padding => 0)

button = []
0.upto(8) do |i|
  button.push Gtk::Button.new(:label => "button"+(i+1).to_s)
end
0.upto(8) do |i|
  button[i].signal_connect("clicked") do |w|
    p [i]
    j = (i + 1) % 9
    if button[j].visible?
      button[j].hide
    else
      button[j].show
    end
  end
  button[i].show
end
table.attach(button[0], 0, 1, 0, 1, nil, nil, 0, 0)
table.attach(button[1], 1, 2, 1, 2, nil, nil, 0, 0)
table.attach(button[2], 2, 3, 2, 3, nil, nil, 0, 0)
table.attach(button[3], 0, 1, 2, 3, nil, nil, 0, 0)
table.attach(button[4], 2, 3, 0, 1, nil, nil, 0, 0)
table.attach(button[5], 1, 2, 2, 3, nil, nil, 0, 0)
table.attach(button[6], 1, 2, 0, 1, nil, nil, 0, 0)
table.attach(button[7], 2, 3, 1, 2, nil, nil, 0, 0)
table.attach(button[8], 0, 1, 1, 2, nil, nil, 0, 0)

separator = Gtk::Separator.new(:vertical)
box1.pack_start(separator, :expand => false, :fill => true, :padding => 0)

box2 = Gtk::Box.new(:vertical, 10)
box2.border_width = 10
box1.pack_start(box2, :expand => false, :fill => true, :padding => 0)

close = Gtk::Button.new(:label => "close")
close.signal_connect("clicked") do
  Gtk.main_quit
end

box2.pack_start(close, :expand => true, :fill => true, :padding => 0)
close.can_default = true
close.grab_default

window.show_all

Gtk.main
