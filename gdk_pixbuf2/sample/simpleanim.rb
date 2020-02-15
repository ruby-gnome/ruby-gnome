#!/usr/bin/env ruby
=begin
  simpleanim.rb - Ruby/GdkPixbuf sample script for Gdk::PixbufSimpleAnim.

  Inspired by http://mail.gnome.org/archives/gtk-perl-list/2005-September/msg00110.html

  Copyright (c) 2002-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.

  $Id: simpleanim.rb,v 1.2 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk3'

$stdout.sync = true

simple_anim = GdkPixbuf::PixbufSimpleAnim.new(64, 64, 24)
store_pixels = []

print 'generating frames'
for red in 0 .. 126
  store_pixels << pixels = ([ 4*(63-red).abs, 0, 0 ] * (64*64)).pack('C*')
  pixbuf = GdkPixbuf::Pixbuf.new(:data => pixels,
                                 :colorspace => :rgb,
                                 :has_alpha =>false,
                                 :bits_per_sample => 8,
                                 :width => 64,
                                 :height => 64,
                                 :row_stride => 64*3)
  simple_anim.add_frame(pixbuf)
  print '.'
end
puts 'done'

window = Gtk::Window.new
image = Gtk::Image.new(animation: simple_anim)
window.add(image)
window.show_all
window.signal_connect('destroy') { Gtk.main_quit }
Gtk.main
