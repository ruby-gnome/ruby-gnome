# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: list_store.rb,v 1.5 2005/02/06 18:25:13 kzys Exp $
=begin
= Tree View/List Store

The Gtk::ListStore is used to store data in list form, to be used
later on by a Gtk::TreeView to display it. This demo builds a
simple Gtk::ListStore and displays it. See the Stock Browser
demo for a more advanced example.
=end
require 'common'

module Demo
  class ListStore < BasicWindow
    Bug = Struct.new('Bug', :fixed, :number, :severity, :description)
    COLUMN_FIXED, COLUMN_NUMBER, COLUMN_SEVERITY, COLUMN_DESCRIPTION,
      NUM_COLUMNS = *(0..5).to_a
    DATA = [
      [ false, 60482, 'Normal',     'scrollable notebooks and hidden tabs' ],
      [ false, 60620, 'Critical',   'gdk_window_clear_area (gdkwindow-win32.c) is not thread-safe' ],
      [ false, 50214, 'Major',      'Xft support does not clean up correctly' ],
      [ true,  52877, 'Major',      'GtkFileSelection needs a refresh method. ' ],
      [ false, 56070, 'Normal',     "Can't click button after setting in sensitive" ],
      [ true,  56355, 'Normal',     'GtkLabel - Not all changes propagate correctly' ],
      [ false, 50055, 'Normal',     'Rework width/height computations for TreeView' ],
      [ false, 58278, 'Normal',     "gtk_dialog_set_response_sensitive  doesn't work" ],
      [ false, 55767, 'Normal',     'Getters for all setters' ],
      [ false, 56925, 'Normal',     'Gtkcalender size' ],
      [ false, 56221, 'Normal',     'Selectable label needs right-click copy menu' ],
      [ true,  50939, 'Normal',     'Add shift clicking to GtkTextView' ],
      [ false, 6112,  'Enhancement','netscape-like collapsable toolbars' ],
      [ false, 1,     'Normal',     'First bug :=)' ],
    ].collect do |ary|
      Bug.new(*ary)
    end
    def initialize
      super('GtkListStore demo')
      self.border_width = 8

      vbox = Gtk::VBox.new(false, 8)
      add(vbox)

      label = Gtk::Label.new('This is the bug list (note: not based on real data, it would be nice to have a nice ODBC interface to bugzilla or so, though).')
      vbox.pack_start(label, false, false, 0)

      sw = Gtk::ScrolledWindow.new(nil, nil)
      sw.shadow_type = Gtk::SHADOW_ETCHED_IN
      sw.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC)
      vbox.pack_start(sw, true, true, 0)

      # create tree model
      model = create_model

      # create tree view
      treeview = Gtk::TreeView.new(model)
      treeview.rules_hint = true
      treeview.search_column = COLUMN_DESCRIPTION

      sw.add(treeview)

      # add columns to the tree view
      add_columns(treeview)

      set_default_size(280, 250)
    end

    def create_model
      # create list store
      store = Gtk::ListStore.new(TrueClass, Integer, String, String)

      # add data to the list store
      DATA.each do |bug|
	iter = store.append
	bug.each_with_index do |value, index|
	  iter[index] = value
	end
      end
      return store
    end

    def add_columns(treeview)
      # column for fixed toggles
      renderer = Gtk::CellRendererToggle.new
      renderer.signal_connect('toggled') do |cell, path|
	fixed_toggled(treeview.model, path)
      end

      column = Gtk::TreeViewColumn.new('Fixed?',
				       renderer,
				       'active' => COLUMN_FIXED)

      # set this column to a fixed sizing (of 50 pixels)
      column.sizing = Gtk::TreeViewColumn::FIXED
      column.fixed_width = 50
      treeview.append_column(column)

      # column for bug numbers
      renderer = Gtk::CellRendererText.new
      column = Gtk::TreeViewColumn.new('Bug number',
				       renderer,
				       'text' => COLUMN_NUMBER)
      column.set_sort_column_id(COLUMN_NUMBER)
      treeview.append_column(column)

      # column for severities
      renderer = Gtk::CellRendererText.new
      column = Gtk::TreeViewColumn.new('Severity',
				       renderer,
				       'text' => COLUMN_SEVERITY)
      column.set_sort_column_id(COLUMN_SEVERITY)
      treeview.append_column(column)

      # column for description
      renderer = Gtk::CellRendererText.new
      column = Gtk::TreeViewColumn.new('Description',
				       renderer,
				       'text' => COLUMN_DESCRIPTION)
      column.set_sort_column_id(COLUMN_DESCRIPTION)
      treeview.append_column(column)
    end

    def fixed_toggled(model, path_str)
      path = Gtk::TreePath.new(path_str)

      # get toggled iter
      iter = model.get_iter(path)
      fixed =iter[COLUMN_FIXED]

      # do something with the value
      fixed ^= 1

      # set new value
      iter[COLUMN_FIXED] = fixed
    end
  end
end
