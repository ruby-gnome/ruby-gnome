#!/usr/bin/env ruby
=begin
  scalebutton.rb - Very simple Gtk::ScaleButton example.

  Copyright (c) 2007 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: scalebutton.rb,v 1.1 2007/07/12 14:04:54 ggc Exp $
=end

require 'gtk2'

if str = Gtk.check_version(2, 12, 0)
    puts "This sample requires GTK+ 2.12.0 or later"
    puts str
    exit
end

window = Gtk::Window.new
window.add(scale = Gtk::ScaleButton.new(Gtk::IconSize::BUTTON))
scale.set_icons(['gtk-goto-bottom', 'gtk-goto-top', 'gtk-execute'])
scale.signal_connect('value-changed') { |widget, value| puts "value changed: #{value}" }
window.show_all

Gtk.main
