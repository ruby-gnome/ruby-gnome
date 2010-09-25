=begin
  tree_combo.rb - Gtk::TreeView and Gtk::CellRendererCombo 
  sample script.

  Copyright (C) 2006 Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: tree_combo.rb,v 1.3 2006/06/17 13:18:12 mutoh Exp $
=end


require 'gtk2'

if str = Gtk.check_version(2, 6, 0)
  puts "This sample requires GTK+ 2.6.0 or later"
  puts str
  exit
end

T_APPLICATION       = 0
T_COMBO_TEXT_COLUMN = 1
T_COMBO_MODEL       = 2
T_COMBO_HAS_ENTRY   = 3
T_COMBO_EDITABLE    = 4
T_COMBO_TEXT        = 5

# The COMPUTER combo model
combo_model = Gtk::ListStore.new(String)
  
["Ruby", "Perl", "Python", "Java"].each do |v|
  iter = combo_model.append
  iter[0] = v
end


# The table model.
model = Gtk::ListStore.new(String,         # T_APPLICATION
			   Integer,        # T_COMBO_TEXT_COLUMN
			   Gtk::ListStore, # T_COMBO_MODEL
			   TrueClass,      # T_COMBO_HAS_ENTRY
			   TrueClass,      # T_COMBO_EDITABLE
			   String)         # T_COMBO_TEXT

view = Gtk::TreeView.new(model)

# 1st column(Text)
trenderer = Gtk::CellRendererText.new
tcol = Gtk::TreeViewColumn.new("Application", trenderer,
			       :text => T_APPLICATION)
view.append_column(tcol)

# 2nd column(Combo)
crenderer = Gtk::CellRendererCombo.new
crenderer.signal_connect("edited") do |renderer, path, text|
  model.get_iter(path)[T_COMBO_TEXT] = text
end

ccol = Gtk::TreeViewColumn.new("Language", crenderer,
			       :text_column => T_COMBO_TEXT_COLUMN, 
			       :model => T_COMBO_MODEL, 
			       :has_entry => T_COMBO_HAS_ENTRY,
			       :editable => T_COMBO_EDITABLE,
			       :text => T_COMBO_TEXT)
view.append_column(ccol)


# Create rows.
(1..2).each do |v|
  iter = model.append
  iter[T_APPLICATION] = "application #{v}"
  iter[T_COMBO_MODEL] = combo_model
  iter[T_COMBO_HAS_ENTRY] = false
  iter[T_COMBO_EDITABLE] = true
  iter[T_COMBO_TEXT] = combo_model.get_iter("0")[0]
end

win = Gtk::Window.new
win.signal_connect("delete_event"){
  Gtk.main_quit
}

win.add(view).show_all

Gtk.main

