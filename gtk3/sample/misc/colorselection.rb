=begin
  colorselection.rb - Gtk::ColorSelection sample script.

  Copyright (C) 2005,2006  Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: colorselection.rb,v 1.3 2006/06/17 13:18:12 mutoh Exp $
=end

require "gtk3"

a = Gtk::ColorSelection.new
a.has_palette = true
a.signal_connect("color_changed") do |w|
  unless w.adjusting?
    p w.current_rgba.to_s
  end
end

Gtk::Window.new.add(a).show_all.signal_connect("destroy") {Gtk.main_quit}

Gtk.main
