#!/usr/bin/env ruby
=begin
  loader.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: loader.rb,v 1.2 2005/03/22 17:41:25 silicio Exp $
=end
require 'gtk2'

Gtk.init
loader = Gdk::PixbufLoader.new
File.open("gnome-foot.png","rb") { |f|
  loader.last_write(f.read)
}
pixbuf = loader.pixbuf

Gtk::Window.new.add(Gtk::Image.new(pixbuf)).show_all

Gtk.main
