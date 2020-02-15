#!/usr/bin/env ruby
=begin
  loader.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2003-2020: Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.

  $Id: loader.rb,v 1.3 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk3'

loader = GdkPixbuf::PixbufLoader.new
File.open(File.join(__dir__, "gnome-foot.png"), "rb") { |f|
  loader.last_write(f.read)
}
pixbuf = loader.pixbuf

window = Gtk::Window.new
window.signal_connect('delete-event') { Gtk.main_quit }
window.add(Gtk::Image.new(pixbuf: pixbuf))
window.show_all

Gtk.main
