#!/usr/local/bin/ruby
=begin
  fileselection.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: fileselection.rb,v 1.5 2003/01/19 14:28:23 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::FileSelection.new("file selection dialog")
window.window_position = Gtk::Window::POS_MOUSE
window.border_width = 0

ret = window.run
if ret == Gtk::Dialog::RESPONSE_OK
  print "OK\n"
  print window.filename, "\n"
elsif ret == Gtk::Dialog::RESPONSE_CANCEL
  print "CANCEL\n"
  print window.filename, "\n"
end
