require 'gtk2'
require 'gdk_pixbuf2'

pixbuf =  Gdk::Pixbuf.new(ARGV[0])

pixmap, mask = pixbuf.scale(500, 200).render_pixmap_and_mask
pw = Gtk::Pixmap.new(pixmap, mask)
pw.show

w = Gtk::Window.new
w.signal_connect('delete-event') do
  Gtk.main_quit
end

w.add pw
w.show

Gtk.main
