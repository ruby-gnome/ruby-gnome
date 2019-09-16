#!/usr/bin/env ruby
=begin
  loader.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2003,2006: Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: loader.rb,v 1.3 2006/06/17 14:38:08 mutoh Exp $
=end
require 'gtk2'

loader = Gdk::PixbufLoader.new
File.open("gnome-foot.png","rb") { |f|
  loader.last_write(f.read)
}
pixbuf = loader.pixbuf

Gtk::Window.new.add(Gtk::Image.new(pixbuf)).show_all

Gtk.main
