require 'gtk'
require 'gdk_pixbuf'

pixbuf =  Gdk::Pixbuf.new(ARGV[0])

w = Gtk::Window.new
w.signal_connect('delete-event') do
  Gtk.main_quit
end

d = Gtk::DrawingArea.new
d.signal_connect('expose-event') do |w, e|
  unless e.count > 0
    pixbuf.scale(w.allocation.width, w.allocation.height).
      render_to_drawable(w.window, w.style.fg_gc(Gtk::STATE_NORMAL), 0, 0,
			 0, 0, w.allocation.width, w.allocation.height)
  end
  GC.start
  true
end
d.show
w.add d
w.show

Gtk.main
