#!/usr/bin/env ruby
=begin
  treeview.rb - Gtk::TreeView sample

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"
window = Gtk::Window.new("Gtk::TreeView sample")

# Create data
model = Gtk::TreeStore.new(String, String, Gdk::RGBA, Float, GdkPixbuf::Pixbuf)

# column 1
root_iter = model.append(nil)
root_iter[0] = "Root"
root_iter[1] = "red"
root_iter[2] = Gdk::RGBA.new(0, 1, 0, 1)
root_iter[3] = 5.0

# column 2
root_iter[4] = window.render_icon_pixbuf(Gtk::Stock::NEW, Gtk::IconSize::DIALOG)

child_iter1 = model.append(root_iter)
child_iter1[0] = "Child_Iter1"
child_iter1[1] = "green"
child_iter1[2] = Gdk::RGBA.new(1, 0, 0, 1)
child_iter1[3] = 3.0

# column 2
child_iter1[4] = window.render_icon_pixbuf(Gtk::Stock::OPEN, Gtk::IconSize::MENU)

child_iter2 = model.append(root_iter)
child_iter2[0] = "Child_Iter2"
child_iter2[1] = "yellow"
child_iter2[2] = Gdk::RGBA.new(0, 0, 1, 1)
child_iter2[3] = 0.9

# column 2
child_iter2[4] = window.render_icon_pixbuf(Gtk::Stock::QUIT, Gtk::IconSize::BUTTON)

# Create view
tv = Gtk::TreeView.new(model)

# column 1
renderer = Gtk::CellRendererText.new
column = Gtk::TreeViewColumn.new("Gtk::CellRenderText", renderer, {
                                   :text => 0,
                                   :background => 1,
                                   :foreground_rgba => 2,
                                   :scale => 3
                                 })
tv.append_column(column)

# column 2
renderer = Gtk::CellRendererPixbuf.new
column = Gtk::TreeViewColumn.new("Gtk::CellRenderPixbuf", renderer, {
                                   :pixbuf => 4
                                 })

tv.append_column(column)

window.add(tv)
window.set_default_size(300, 300)
window.show_all
window.signal_connect("destroy") { Gtk.main_quit }

Gtk.main
