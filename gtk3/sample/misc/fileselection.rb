#!/usr/bin/env ruby
=begin
  fileselection.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: fileselection.rb,v 1.10 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

puts "Deprecated. Use Gtk::FileChooserDialog instead."

window = Gtk::FileSelection.new("file selection dialog")
window.window_position = Gtk::Window::POS_MOUSE
window.border_width = 0

ret = window.run
if ret == Gtk::Dialog::RESPONSE_OK
  puts "OK"
  puts window.filename
elsif ret == Gtk::Dialog::RESPONSE_CANCEL
  puts "CANCEL"
  puts window.filename
end
