=begin
  label.rb - Ruby/Pango sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: label.rb,v 1.2 2003/02/01 17:13:24 mutoh Exp $
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
