#!/usr/local/bin/ruby
=begin
  toolbar.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2004 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: toolbar.rb,v 1.8 2004/03/05 16:24:30 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.set_title("toolbar test")

window.signal_connect("destroy") do
  Gtk.main_quit
end
window.set_border_width(0)
window.realize

toolbar = Gtk::Toolbar.new

toolbar.append "Horizontal", "Horizontal toolbar layout", "Toolbar/Horizontal",
  Gtk::Image.new("test.xpm") do
  toolbar.set_orientation Gtk::ORIENTATION_HORIZONTAL
end
toolbar.append "Vertical", "Vertical toolbar layout", "Toolbar/Vertical",
  Gtk::Image.new("test.xpm") do
  toolbar.set_orientation Gtk::ORIENTATION_VERTICAL
end
toolbar.append_space
toolbar.append "Icons", "Only show toolbar icons", "Toolbar/IconsOnly",
  Gtk::Image.new("test.xpm") do
  toolbar.set_toolbar_style Gtk::Toolbar::ICONS
end
toolbar.append "Text", "Only show toolbar text", "Toolbar/TextOnly",
  Gtk::Image.new("test.xpm") do
  toolbar.set_toolbar_style Gtk::Toolbar::TEXT
end
toolbar.append "Both", "Show toolbar icons and text", "Toolbar/Both",
  Gtk::Image.new("test.xpm") do
  toolbar.set_toolbar_style Gtk::Toolbar::BOTH
end
toolbar.append_space
toolbar.append "Enable", "Enable tooltips",nil,
  Gtk::Image.new("test.xpm") do
  toolbar.set_tooltips true
end
toolbar.append "Disable", "Disable tooltips",nil,
  Gtk::Image.new("test.xpm") do
  toolbar.set_tooltips false
end
window.add toolbar
window.show_all

Gtk.main
