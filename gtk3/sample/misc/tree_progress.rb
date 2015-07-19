=begin
  tree_progress.rb - Gtk::TreeView and Gtk::CellRendererProgress
  sample script.

  Copyright (C) 2004-2015 Darren Willis, Masao Mutoh
  Copyright (c) 2004-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

win = Gtk::Window.new("Gtk::CellRendererProgress sample")
win.set_default_size(300, 50)
model = Gtk::ListStore.new(String, Float)
view = Gtk::TreeView.new(model)

prog = model.append
prog[0] = "bar 1"
prog[1] = 50

prog1 = model.append
prog1[0] = "bar 2"
prog1[1] = 5

prog2 = model.append
prog2[0] = "bar 3"
prog2[1] = 10

arenderer = Gtk::CellRendererText.new
acol = Gtk::TreeViewColumn.new("words", arenderer, :text  => 0)
view.append_column(acol)

prenderer = Gtk::CellRendererProgress.new
pcol = Gtk::TreeViewColumn.new("Progress", prenderer, :value  => 1)
view.append_column(pcol)

win.signal_connect("delete_event") do
  Gtk.main_quit
end

win.add(view)
win.show_all

dir = 1
GLib::Timeout.add(100) do
  value = prog2[1] + dir

  dir = - dir if value > 100
  dir = - dir if value < 0

  prog2[1] += dir

  GLib::Source::CONTINUE
end

Gtk.main
