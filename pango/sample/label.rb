=begin
  label.rb - Ruby/Pango sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: label.rb,v 1.1 2003/01/03 16:34:48 mutoh Exp $
=end

require 'gtk2'

Gtk.init

str = File.read("sample.txt")
attrs, text, accel_char = Pango.parse_markup(str, '$')

#This is same as Gtk::Label#set_markup()
label = Gtk::Label.new(text)
label.attributes = attrs

Gtk::Window.new.add(label).show_all

Gtk.main
