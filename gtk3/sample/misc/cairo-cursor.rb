=begin
  cairo_cursor.rb Ruby/GTK3 script
  Adapted from https://developer.gnome.org/gtk3/stable/ch25s02.html#id-1.6.3.4.5
  Copyright (c) 2015-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "gtk3"

window = Gtk::Window.new("Gdk cursor sample")
window.signal_connect("destroy") { Gtk.main_quit }

# see cairo formats information here:
# http://cairographics.org/manual/cairo-Image-Surfaces.html#cairo-format-t
surface = Cairo::ImageSurface.new(Cairo::FORMAT_ARGB32, 6, 6)

cr = Cairo::Context.new(surface)
cr.set_source_rgba(0, 1, 0, 0.7)
cr.arc(3, 3, 3, 0, 2 * Math::PI)
cr.fill
cr.destroy

pixbuf = surface.to_pixbuf(src_x: 0, src_y: 0, width: 6, heihgt: 6)
cursor = Gdk::Cursor.new(pixbuf, 0, 0)

# generate the underlaying GDK resource associated with the window widget.
# Here just create window.window without calling #show_all method.
window.realize
window.window.set_cursor(cursor)

label = Gtk::Label.new("Put your cursor on this window.")
window.add(label)

window.set_default_size(200, 100)
window.show_all

Gtk.main
