#!/usr/bin/env ruby
=begin
  composite.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.

  $Id: composite.rb,v 1.6 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk3'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

src =  GdkPixbuf::Pixbuf.new(:file => filename)

vbox = Gtk::Box.new(:vertical)

dst = src.composite(:dest_width  => 100,
                    :dest_height => 100,
                    :interp_type => :hyper,
                    :overall_alpha => 255,
                    :check_size => 32,
                    :color1 => 0xFF0000,
                    :color2 => 0x00FF00)
vbox.pack_start(Gtk::Image.new(:pixbuf => dst))

dst = GdkPixbuf::Pixbuf.new(:colorspace => :rgb,
                            :has_alpha => true,
                            :bits_per_sample => 8,
                            :width =>200,
                            :height => 200)
dst.composite!(src,
               :dest_width => 200,
               :dest_height => 200,
               :scale_x => 1.8,
               :scale_y => 1.8,
               :interp_type => :hyper,
               :overall_alpha => 200)
vbox.pack_start(Gtk::Image.new(:pixbuf => dst))

dst = GdkPixbuf::Pixbuf.new(:colorspace => :rgb,
                            :has_alpha => true,
                            :bits_per_sample => 8,
                            :width =>200,
                            :height => 200)
dst.composite!(src,
               :dest_x => 10,
               :dest_y => 10,
               :dest_width => 180,
               :dest_height => 180,
               :offset_x => 15,
               :offset_y => 15,
               :scale_x => 3,
               :scale_y => 2,
               :overall_alpha => 200,
               :check_x => 100,
               :check_y => 100,
               :check_size => 16,
               :color1 => 0x999999,
               :color2 => 0xdddddd)
vbox.pack_start(Gtk::Image.new(:pixbuf => dst))

window = Gtk::Window.new.add(vbox)
window.signal_connect('delete-event') do
  Gtk.main_quit
end

window.show_all

Gtk.main
