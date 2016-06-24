#!/usr/bin/env ruby
=begin
  iconview.rb - Ruby/GTK3 sample script.

  Copyright (c) 2005-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

TEXT_COLUMN   = 0
PIXBUF_COLUMN = 1

model = Gtk::ListStore.new(String, GdkPixbuf::Pixbuf)

iv = Gtk::IconView.new
iv.set_model(model)
iv.text_column   = TEXT_COLUMN
iv.pixbuf_column = PIXBUF_COLUMN

Dir.glob("#{File.expand_path(File.dirname(__FILE__))}/../gtk-demo/gnome*.png").each do |f|
  iter = model.append
  iter[TEXT_COLUMN]   = File.basename(f)
  iter[PIXBUF_COLUMN] = GdkPixbuf::Pixbuf.new(:file => f)
end

iv.signal_connect("item_activated") do |iv, path|
  iter = model.get_iter(path)
  p iter [0]
end

win = Gtk::Window.new("Gtk::IconView sample")
win.signal_connect("destroy"){Gtk.main_quit}
win.add(iv)
win.set_default_size(300,300)
win.show_all

Gtk.main
