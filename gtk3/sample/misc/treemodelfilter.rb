#!/usr/bin/env ruby
=begin
  treemodelfilter.rb - Gtk::TreeModelFilter sample

  Copyright (c) 2004-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

ls = Gtk::ListStore.new(String, Integer)

column1 = Gtk::TreeViewColumn.new("data1",
                                  Gtk::CellRendererText.new,
                                  :text => 0)
column2 = Gtk::TreeViewColumn.new("data2",
                                  Gtk::CellRendererText.new,
                                  :text => 1)

mf = ls.create_filter

mf.set_visible_func do |_model, iter|
  /a/ =~ iter[0]
end

mf.set_modify_func(String, String) do |model, iter, column|
  child_iter = model.convert_iter_to_child_iter(iter)
  if column == 0
    child_iter[0]
  else
    "#{child_iter[1]} * 2 = #{child_iter[1] * 2}"
  end
end

treeview = Gtk::TreeView.new(mf)
treeview.append_column(column1)
treeview.append_column(column2)

[
  ["aaa", 1], ["aab", 2], ["aac", 3],
  ["bba", 4], ["bbb", 5], ["bbc", 6]
].each do |str, num|
  iter = ls.append
  iter[0] = str
  iter[1] = num
end

win = Gtk::Window.new("Gtk::TreeModelFilter sample")
win.signal_connect("destroy") { Gtk.main_quit }
win.add(treeview)
win.show_all
Gtk.main
