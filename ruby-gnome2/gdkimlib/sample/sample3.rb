#!/usr/local/bin/ruby

require 'gtk'
require 'gdk_imlib'

#
# Ruby/Gtk + Imlib : rotate demo
#

class Rotate
  def initialize(filename)
    @pixbox = nil
    @im = nil
    @pixmap = nil

    window = Gtk::Window.new(Gtk::WINDOW_TOPLEVEL)
    window.set_title(filename)
    window.signal_connect('destroy'){ exit }
    window.signal_connect('delete_event'){ exit }

    window = Gtk::Window.new(Gtk::WINDOW_TOPLEVEL)
    window.set_title(filename)
    window.signal_connect('destroy'){ exit }
    window.signal_connect('delete_event'){ exit }

    vbox = Gtk::VBox.new(false, 0)
    window.add(vbox)
    vbox.show

    @pixbox = Gtk::VBox.new(false, 0)
    vbox.pack_start(@pixbox, true, true, 0)
    @pixbox.show

    @im = GdkImlib::Image.new(filename)
    @im.render(@im.rgb_width, @im.rgb_height)
    pixd, maskd = @im.get_pixmap
    @pixmap = Gtk::Pixmap.new(pixd, maskd)
    @pixmap.show
    @pixbox.pack_start(@pixmap, false, false, 0)

    hbox = Gtk::HBox.new(true, 0)
    vbox.pack_start(hbox, false, false, 0)
    hbox.show

    button = Gtk::Button.new("right rotate")
    button.signal_connect('clicked'){ right_rotate }
    button.show
    hbox.pack_start(button, false, true, 0)

    button = Gtk::Button.new("left rotate")
    button.signal_connect('clicked'){ left_rotate }
    button.show
    hbox.pack_start(button, false, true, 0)

    window.show
  end

  def right_rotate
    @pixbox.remove(@pixmap)

    @im.flip_vertical
    @im.rotate(1)

    @im.render(@im.rgb_width, @im.rgb_height)
    pixd, maskd = @im.get_pixmap
    @pixmap = Gtk::Pixmap.new(pixd, maskd)
    @pixmap.show
    @pixbox.pack_start(@pixmap, false, false, 0)
  end

  def left_rotate
    @pixbox.remove(@pixmap)

    @im.flip_horizontal
    @im.rotate(1)

    @im.render(@im.rgb_width, @im.rgb_height)
    pixd, maskd = @im.get_pixmap
    @pixmap = Gtk::Pixmap.new(pixd, maskd)
    @pixmap.show
    @pixbox.pack_start(@pixmap, false, false, 0)
  end
end

if(ARGV.size != 1)
  print $0, " filename\n"
  exit
end

Rotate.new(ARGV.shift)

Gtk.main
