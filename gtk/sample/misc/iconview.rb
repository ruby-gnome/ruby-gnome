#!/usr/bin/env ruby
=begin
  iconview.rb - Ruby/GTK sample script.

  Copyright (c) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: iconview.rb,v 1.1 2005/10/26 16:31:27 mutoh Exp $
=end

require 'gtk2'

Gtk.init

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

Gtk::Window.new.add(iv).set_default_size(300,300).show_all

Gtk.main
