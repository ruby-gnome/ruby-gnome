#!/usr/local/bin/ruby

require 'gtk'
require 'gdk_imlib'

class ImgWin
  def initialize(filename)
    window = Gtk::Window.new(Gtk::WINDOW_TOPLEVEL)
    window.set_title(filename)
    window.signal_connect('destroy'){
      window.destroy
    }
    window.signal_connect('delete_event'){
      window.destroy
    }

    window.signal_connect('configure_event'){|widget, event|
      resize(event.width, event.height)
    }

    @vbox = Gtk::VBox.new(false, 0)
    window.add(@vbox)
    @vbox.show
    @im = GdkImlib::Image.new(filename)
    @im.render(@im.rgb_width, @im.rgb_height)
    pixd, maskd = @im.get_pixmap
    pixmap = Gtk::Pixmap.new(pixd, maskd)
    pixmap.show
    @vbox.pack_start(pixmap, true, true, 0)
    window.show
  end

  def resize(width, height)
    for cc in @vbox.children
      @vbox.remove(cc)
    end
    @im.render(width, height)
    pixd, maskd = @im.get_pixmap
    pixmap = Gtk::Pixmap.new(pixd, maskd)
    @vbox.pack_start(pixmap, true, true, 0)
    pixmap.show
#    GC.start
  end
end

fs = Gtk::FileSelection.new("Image viewer");
fs.signal_connect('destroy'){ exit }
fs.signal_connect('delete_event'){ exit }

fs.cancel_button.signal_connect('clicked') {
  exit
}

fs.ok_button.signal_connect('clicked'){
  ImgWin.new(fs.get_filename)
}

fs.show
Gtk.main
