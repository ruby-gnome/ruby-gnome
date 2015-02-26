#!/usr/bin/env ruby
=begin
  listview.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("Gtk::ListStore sample")
window.border_width = 0

box1 = Gtk::Box.new(:vertical, 0)
window.add(box1)

box2 = Gtk::Box.new(:vertical, 10)
box2.border_width = 10
box1.pack_start(box2, :expand => true, :fill => true, :padding => 0)

scrolled_win = Gtk::ScrolledWindow.new
scrolled_win.set_policy(:automatic,:automatic)
box2.pack_start(scrolled_win, :expand => true, :fill => true, :padding => 0)

data = [
    "hello",
    "world",
    "blah",
    "foo",
    "bar",
    "argh",
    "spencer",
    "is a",
    "wussy",
    "programmer"
  ]

model = Gtk::ListStore.new(String)
column = Gtk::TreeViewColumn.new("Data",
                                 Gtk::CellRendererText.new, {:text => 0})
treeview = Gtk::TreeView.new(model)
treeview.append_column(column)
treeview.selection.set_mode(:single)
scrolled_win.add_with_viewport(treeview)

data.each do |v|
  iter = model.append
  iter[0] =  v
end

button = Gtk::Button.new(:label => "add")
button.can_focus=true

i = 0
button.signal_connect("clicked") do
  iter = model.append
  iter[0] =  "add item #{i}"
  i += 1
end

box2.pack_start(button, :expand => false, :fill => true, :padding => 0)

button = Gtk::Button.new(:label => "remove")
button.can_focus=true
button.signal_connect("clicked") do
  iter = treeview.selection.selected
  model.remove(iter) if iter
end
box2.pack_start(button, :expand => false, :fill => true, :padding => 0)

separator = Gtk::Separator.new(:horizontal)
box1.pack_start(separator, :expand => false, :fill => true, :padding => 0)
separator.show

box2 = Gtk::Box.new(:vertical, 10)
box2.border_width = 10
box1.pack_start(box2, :expand => false, :fill => true, :padding => 0)

button = Gtk::Button.new(:label => "close")
button.signal_connect("clicked") do
  Gtk.main_quit
end
box2.pack_start(button, :expand => true, :fill => true, :padding => 0)
button.can_default=true
button.grab_default

window.set_default_size(300, 300)
window.show_all

Gtk.main
