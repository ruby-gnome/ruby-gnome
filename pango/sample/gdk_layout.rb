require 'gtk2'

Gtk.init
window = Gtk::Window.new
window.realize

gdkwin = window.window
gc = Gdk::GC.new(gdkwin)

layout = window.create_pango_layout
layout.set_markup(File.read("sample.txt"), "$")
window.signal_connect("expose_event") do |v|
  gdkwin.draw_layout(gc, 10, 10, layout)
end

window.set_default_size(500,400).show_all

Gtk.main
