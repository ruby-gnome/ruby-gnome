#!/usr/bin/env ruby
=begin
  misc_button.rb - Ruby/GTK sample script.
                                                                                
  Copyright (c) 2004-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
                                                                                
  $Id: misc_button.rb,v 1.2 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk3'

box = Gtk::Box.new(:vertical)

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

#Gtk::FileChooserButton
filebutton = Gtk::FileChooserButton.new("Gtk::FileChooserButton", 
                                    Gtk::FileChooser::Action::OPEN)
filebutton.filename = GLib.home_dir
filebutton.signal_connect("current-folder-changed") do |w, e|
  p filebutton.filename
end

box.add(filebutton)

#Quit
quitbutton = Gtk::Button.new(:label => "Quit")
quitbutton.signal_connect("clicked") do
  Gtk.main_quit
end
box.add(quitbutton)

Gtk::Window.new.add(box).set_default_size(200, 100).show_all

Gtk.main
