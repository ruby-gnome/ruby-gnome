#!/usr/bin/env ruby
=begin
  anim.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: anim.rb,v 1.4 2005/03/22 17:41:24 silicio Exp $
=end

require 'gtk2'

Gtk.init

w = Gtk::Window.new
w.signal_connect('delete-event') do
  Gtk.main_quit
end

box = Gtk::VBox.new
src =  Gdk::PixbufAnimation.new("floppybuddy.gif")
box.pack_start(Gtk::Image.new(src))
p src.width
p src.height
p src.static_image?

static_image = src.static_image
box.pack_start(Gtk::Image.new(static_image))

iter = src.get_iter
p iter.advance
p iter.delay_time
p iter.on_currently_loading_frame?

box.pack_start(Gtk::Image.new(iter.pixbuf))

w.add(box)
w.show_all

Gtk.main
