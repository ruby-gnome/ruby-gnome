#!/usr/bin/env ruby
=begin
  treestore.rb - Gtk::TreeStore sample

  Copyright (c) 2016 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
  Based on  https://developer.gnome.org/gtk3/stable/TreeWidget.html
=end

require "gtk3"

TITLE_COLUMN = 0
AUTHOR_COLUMN = 1
CHECKED_COLUMN = 2

model = Gtk::TreeStore.new(String, String, TrueClass)
iter = model.append(nil)
iter[TITLE_COLUMN] = "The Principle of Reason"
iter[AUTHOR_COLUMN] = "Martin Heidegger"
iter[CHECKED_COLUMN] = false

iter = model.append(nil)
iter.set_values(["The Art of Computer Programming", "Donald E. Knuth", true])

# append child
child_iter = model.append(iter)
child_iter[TITLE_COLUMN] = "Volume 1: Fundamental Algorithms"

# append another child
child_iter = model.append(iter)
child_iter.set_value(TITLE_COLUMN, "Volume 2: Seminumerical Algorithms")
child_iter.set_value(CHECKED_COLUMN, true)

# append another child
child_iter = model.append(iter)
child_iter.set_values(["Volume 3: Sorting and Searching",
                       "Donald E. Knuth",
                       true])

def treeview_widget_of(model)
  treeview = Gtk::TreeView.new(model)
  treeview.append_column(Gtk::TreeViewColumn.new("Title",
                                                 Gtk::CellRendererText.new,
                                                 :text => TITLE_COLUMN))
  treeview.append_column(Gtk::TreeViewColumn.new("Author",
                                                 Gtk::CellRendererText.new,
                                                 :text => AUTHOR_COLUMN))
  treeview.append_column(Gtk::TreeViewColumn.new("Author",
                                                 Gtk::CellRendererToggle.new,
                                                 :active => CHECKED_COLUMN))
  treeview
end

def display_model(model)
  win = Gtk::Window.new(:toplevel)
  win.title = "Simple Gtk::TreeStore"
  win.add(treeview_widget_of(model))
  win.show_all
  win.signal_connect("destroy") { Gtk.main_quit }
  Gtk.main
end

display_model(model)
