#!/usr/local/bin/ruby
=begin
  button.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: button.rb,v 1.7 2003/05/04 03:49:58 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.set_title("buttons")
window.set_border_width(0)

box1 = Gtk::VBox.new(false, 0)
window.add(box1)
box1.show

table = Gtk::Table.new(3, 3, false)
table.set_row_spacings(5)
table.set_column_spacings(5)
table.set_border_width(10)
box1.pack_start(table, true, true, 0)
table.show

button = []
0.upto(8) do |i|
  button.push Gtk::Button.new("button"+(i+1).to_s)
end
0.upto(8) do |i|
  button[i].signal_connect("clicked") do |w|
    p [i]
    j = (i+1)%9
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

separator = Gtk::HSeparator.new
box1.pack_start(separator, false, true, 0)
separator.show

box2 = Gtk::VBox.new(false, 10)
box2.set_border_width(10)
box1.pack_start(box2, false, true, 0)
box2.show

close = Gtk::Button.new("close")
close.signal_connect("clicked") do
	Gtk.main_quit
end
box2.pack_start(close, true, true, 0)
close.set_flags(Gtk::Widget::CAN_DEFAULT)
close.grab_default
close.show

window.show

Gtk.main
