=begin
  colorselection.rb - Gtk::ColorSelection sample script.

  Copyright (C) 2005  Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: colorselection.rb,v 1.1 2005/03/22 15:42:55 mutoh Exp $
=end

require 'gtk2'

Gtk.init

settings = Gtk::Settings.default

# You can save the palette to use right click on the palette.
Gtk::ColorSelection.set_change_palette_hook{|screen, colors|
  puts strs = Gtk::ColorSelection.palette_to_string(colors)
  settings.gtk_color_palette = strs
}

a = Gtk::ColorSelection.new
a.has_palette = true

Gtk::Window.new.add(a).show_all

Gtk.main
