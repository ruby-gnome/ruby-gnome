=begin
  colorselection.rb - Gtk::ColorSelection sample script.

  Copyright (C) 2005-2015  Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

a = Gtk::ColorSelection.new
a.has_palette = true
a.signal_connect("color_changed") do |w|
  unless w.adjusting?
    p w.current_rgba.to_s
  end
end

win = Gtk::Window.new
win.add(a)
win.show_all
win.signal_connect("destroy") {Gtk.main_quit}

Gtk.main
