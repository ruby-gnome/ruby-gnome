#!/usr/bin/env ruby
=begin
  treeview.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: treeview.rb,v 1.6 2005/03/22 17:41:28 silicio Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new

# Create data
model = Gtk::TreeStore.new(String, String, Gdk::Color, Float, Gdk::Pixbuf)

# column 1
root_iter = model.append(nil)
root_iter[0] = "Root"
root_iter[1] = "red"
root_iter[2] = Gdk::Color.new(0, 65535, 0)
root_iter[3] = 5.0
 
# column 2
root_iter[4] = window.render_icon(Gtk::Stock::NEW, Gtk::IconSize::DIALOG, "icon1")

child_iter1 = model.append(root_iter)
child_iter1[0] = "Child_Iter1"
child_iter1[1] = "green"
child_iter1[2] = Gdk::Color.new(65535, 0, 0)
child_iter1[3] = 3.0

# column 2
child_iter1[4] = window.render_icon(Gtk::Stock::OPEN, Gtk::IconSize::MENU, "icon2")

child_iter2 = model.append(root_iter)
child_iter2[0] = "Child_Iter2"
child_iter2[1] = "yellow"
child_iter2[2] = Gdk::Color.new(0, 0, 65535)
child_iter2[3] = 0.9

# column 2
child_iter2[4] = window.render_icon(Gtk::Stock::QUIT, Gtk::IconSize::BUTTON, "icon3")


# Create view
tv = Gtk::TreeView.new(model)

# column 1
renderer = Gtk::CellRendererText.new
column = Gtk::TreeViewColumn.new("Gtk::CellRenderText", renderer, {
                                   :text => 0, 
                                   :background => 1,
                                   :foreground_gdk => 2,
                                   :scale => 3,
                                 })
tv.append_column(column)

# column 2
renderer = Gtk::CellRendererPixbuf.new
column = Gtk::TreeViewColumn.new("Gtk::CellRenderPixbuf", renderer, {
                                   :pixbuf => 4 
                                 })

tv.append_column(column)

window.add(tv).set_default_size(300, 300).show_all

Gtk.main
