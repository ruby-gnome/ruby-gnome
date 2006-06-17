=begin
  gdk_layout.rb - Ruby/Pango sample script.

  Copyright (c) 2003-2006 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: gdk_layout.rb,v 1.7 2006/06/17 13:36:11 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Ruby/Pango sample")
window.signal_connect("destroy"){Gtk.main_quit}
window.realize

gdkwin = window.window
gc = Gdk::GC.new(gdkwin)

layout = window.create_pango_layout
layout.set_markup(File.readlines("sample.txt").to_s, "$")
window.signal_connect("expose_event") do
  gdkwin.draw_layout(gc, 10, 10, layout)
end

window.set_default_size(500,400).show_all

Gtk.main
