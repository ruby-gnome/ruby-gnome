=begin
  tree_progress.rb - Gtk::TreeView and Gtk::CellRendererProgress 
  sample script.

  Copyright (C) 2004-2006 Darren Willis, Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: tree_progress.rb,v 1.3 2006/06/17 13:18:12 mutoh Exp $
=end


require 'gtk2'

if str = Gtk.check_version(2, 6, 0)
  puts "This sample requires GTK+ 2.6.0 or later"
  puts str
  exit
end

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

win.signal_connect("delete_event"){
  Gtk.main_quit
}

win.add(view)
win.show_all

dir = 1
thr = Thread.new do
  loop {
    prog2[1] += dir
    if prog2[1] > 100
      prog2[1] = 100
      dir = - dir
    end
    if prog2[1] < 0
      prog2[1] = 0
      dir = - dir
    end
    sleep 0.01
  }
end

Gtk.main

