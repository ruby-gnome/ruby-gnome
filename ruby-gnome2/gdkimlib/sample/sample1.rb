#!/usr/local/bin/ruby

require 'gtk'
require 'gdk_imlib'

#
# Ruby/Gtk + Imlib : view demo
#

if(ARGV.size != 1)
  print $0, " filename\n"
  exit
end
filename = ARGV.shift

window = Gtk::Window.new(Gtk::WINDOW_TOPLEVEL)
window.set_title(filename)
window.signal_connect('destroy'){ exit }
window.signal_connect('delete_event'){ exit }

vbox = Gtk::VBox.new(false, 0)
window.add(vbox)
vbox.show

im = GdkImlib::Image.new(filename)
im.render(im.rgb_width, im.rgb_height)
pixd, maskd = im.get_pixmap
pixmap = Gtk::Pixmap.new(pixd, maskd)
pixmap.show
vbox.pack_start(pixmap, true, true, 0)

window.show
Gtk.main
