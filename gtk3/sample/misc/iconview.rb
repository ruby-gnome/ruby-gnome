#!/usr/bin/env ruby
=begin
  iconview.rb - Ruby/GTK2 sample script.

  Copyright (c) 2005,2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: iconview.rb,v 1.2 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

TEXT_COLUMN   = 0
PIXBUF_COLUMN = 1

model = Gtk::ListStore.new(String, Gdk::Pixbuf)

iv = Gtk::IconView.new(model)

iv.text_column   = TEXT_COLUMN
iv.pixbuf_column = PIXBUF_COLUMN

Dir.glob("../gtk-demo/gnome*.png").each do |f|
  iter = model.append
  iter[TEXT_COLUMN]   = File.basename(f)
  iter[PIXBUF_COLUMN] = Gdk::Pixbuf.new(f)
end

iv.signal_connect("item_activated") do |iv, path|
  iter = model.get_iter(path)
  p iter [0]
end

win = Gtk::Window.new("Gtk::IconView sample")
win.signal_connect("destroy"){Gtk.main_quit}
win.add(iv).set_default_size(300,300).show_all

Gtk.main
