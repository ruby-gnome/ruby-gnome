#!/usr/bin/env ruby
=begin
  anim.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.

  $Id: anim.rb,v 1.5 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk3'

w = Gtk::Window.new
w.signal_connect('delete-event') do
  Gtk.main_quit
end

box = Gtk::Box.new(:vertical)
src = GdkPixbuf::PixbufAnimation.new(File.join(__dir__, "floppybuddy.gif"))
box.pack_start(Gtk::Image.new(animation: src))
p src.width
p src.height
p src.static_image?

static_image = src.static_image
box.pack_start(Gtk::Image.new(pixbuf: static_image))

iter = src.get_iter
p iter.advance
p iter.delay_time
p iter.on_currently_loading_frame?

box.pack_start(Gtk::Image.new(pixbuf: iter.pixbuf))

w.add(box)
w.show_all

Gtk.main
