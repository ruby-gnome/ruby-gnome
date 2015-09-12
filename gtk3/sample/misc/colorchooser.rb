=begin
  colorchooser.rb - Gtk::ColorChooserWidget sample script.

  Copyright (c) 2015  Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

color_selection = Gtk::ColorChooserWidget.new
color_selection.use_alpha = true
color_selection.signal_connect "color-activated" do |widget, color|
  p color.to_s
end

window = Gtk::Window.new
window.border_width = 10
window.add(color_selection)
window.show_all
window.signal_connect("destroy") {Gtk.main_quit}

Gtk.main
