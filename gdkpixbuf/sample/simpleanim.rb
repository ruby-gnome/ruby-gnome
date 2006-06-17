#!/usr/bin/env ruby
=begin
  simpleanim.rb - Ruby/GdkPixbuf sample script for Gdk::PixbufSimpleAnim.
  
  Inspired by http://mail.gnome.org/archives/gtk-perl-list/2005-September/msg00110.html

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: simpleanim.rb,v 1.2 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk2'

$stdout.sync = true

simple_anim = Gdk::PixbufSimpleAnim.new(64, 64, 24)
store_pixels = []

print 'generating frames'
for red in 0 .. 126
    store_pixels << pixels = ([ 4*(63-red).abs, 0, 0 ] * (64*64)).pack('C*')
    pixbuf = Gdk::Pixbuf.new(pixels, Gdk::Pixbuf::COLORSPACE_RGB, false, 8, 64, 64, 64*3)
    simple_anim.add_frame(pixbuf)
    print '.'
end
puts 'done'

window = Gtk::Window.new
image = Gtk::Image.new(simple_anim)
window.add(image)
window.show_all
window.signal_connect('destroy') { Gtk.main_quit }
Gtk.main
