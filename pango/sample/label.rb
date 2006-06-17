=begin
  label.rb - Ruby/Pango sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: label.rb,v 1.4 2006/06/17 13:36:11 mutoh Exp $
=end

require 'gtk2'

str = File.readlines("sample.txt").to_s
attrs, text, accel_char = Pango.parse_markup(str, '$')

#This is same as Gtk::Label#set_markup()
label = Gtk::Label.new(text)
label.attributes = attrs

win = Gtk::Window.new
win.signal_connect("destroy"){Gtk.main_quit}
win.add(label).show_all

Gtk.main
