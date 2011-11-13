#!/usr/bin/env ruby
=begin
  entry.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: entry.rb,v 1.9 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("entry")
window.border_width = 0

box1 = Gtk::VBox.new(false, 10)
window.add(box1)

entry = Gtk::Entry.new

box1.add(entry)
box1.add(Gtk::HSeparator.new)

button = Gtk::Button.new("close")
button.signal_connect("clicked") do
  Gtk.main_quit
end
box1.add(button)

window.show_all

entry.text = "hello world"

Gtk.main
