#!/usr/local/bin/ruby
=begin
  fileselection.rb - Ruby/GTK sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: fileselection.rb,v 1.1 2002/11/03 18:04:42 mutoh Exp $
=end

require 'gtk2'

window = Gtk::FileSelection::new("file selection dialog")
window.window_position = Gtk::Window::POS_MOUSE
window.border_width = 0

window.ok_button.signal_connect("clicked") do
  print window.get_filename, "\n"
end
window.cancel_button.signal_connect("clicked") do
  window.destroy
  exit
end
window.show

Gtk.main
