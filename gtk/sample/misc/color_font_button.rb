#!/usr/bin/env ruby
=begin
  color_font_button.rb - Ruby/GTK sample script.
                                                                                
  Copyright (c) 2004 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
                                                                                
  $Id: color_font_button.rb,v 1.1 2004/05/30 16:41:13 mutoh Exp $
=end

require 'gtk2'

Gtk.init

box = Gtk::VBox.new

#Gtk::ColorButton
colorbutton = Gtk::ColorButton.new
colorbutton.signal_connect("color-set") do
  p colorbutton.color.to_a
end
box.add(colorbutton)


#Gtk::FontButton
fontbutton = Gtk::FontButton.new
fontbutton.signal_connect("font-set") do
  p fontbutton.font_name
end
box.add(fontbutton)

#Quit
quitbutton = Gtk::Button.new("Quit")
quitbutton.signal_connect("clicked") do
  Gtk.main_quit
end
box.add(quitbutton)

Gtk::Window.new.add(box).show_all

Gtk.main
