#!/usr/bin/env ruby
=begin
  listview.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: listview.rb,v 1.11 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Gtk::ListStore sample")
window.border_width = 0

box1 = Gtk::VBox.new(false, 0)
window.add(box1)

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2, true, true, 0)

scrolled_win = Gtk::ScrolledWindow.new
scrolled_win.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC)
box2.pack_start(scrolled_win, true, true, 0)

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
treeview.selection.set_mode(Gtk::SELECTION_SINGLE)
scrolled_win.add_with_viewport(treeview)

data.each do |v|
  iter = model.append
  iter[0] =  v
end

button = Gtk::Button.new("add")
button.set_flags(Gtk::Widget::CAN_FOCUS)

i = 0
button.signal_connect("clicked") do
  iter = model.append
  iter[0] =  "add item #{i}"
  i += 1
end

box2.pack_start(button, false, true, 0)

button = Gtk::Button.new("remove")
button.set_flags(Gtk::Widget::CAN_FOCUS)
button.signal_connect("clicked") do
  iter = treeview.selection.selected
  model.remove(iter) if iter
end
box2.pack_start(button, false, true, 0)

separator = Gtk::HSeparator.new
box1.pack_start(separator, false, true, 0)
separator.show

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2, false, true, 0)

button = Gtk::Button.new("close")
button.signal_connect("clicked") do
  Gtk.main_quit
end
box2.pack_start(button, true, true, 0)
button.set_flags(Gtk::Widget::CAN_DEFAULT)
button.grab_default

window.set_default_size(300, 300)
window.show_all

Gtk.main
