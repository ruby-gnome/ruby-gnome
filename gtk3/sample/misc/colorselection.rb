=begin
  colorselection.rb - Gtk::ColorSelection sample script.

  Copyright (C) 2005-2015  Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

color_selection = Gtk::ColorSelection.new
color_selection.has_palette = true
color_selection.signal_connect("color_changed") do |widget|
  unless widget.adjusting?
    p widget.current_rgba.to_s
  end
end

window = Gtk::Window.new
window.add(color_selection)
window.show_all
window.signal_connect("destroy") {Gtk.main_quit}

Gtk.main
