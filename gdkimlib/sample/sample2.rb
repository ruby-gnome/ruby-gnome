#!/usr/local/bin/ruby

require 'gtk'
require 'gdk_imlib'

#
# Ruby/Gtk + Imlib : resize demo.
#

class Resize
  def initialize(filename)
    @pixbox = nil
    @im = nil
    @pixmap = nil
    @width = nil
    @height = nil

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

    @width = @im.rgb_width
    @height = @im.rgb_height

    hbox = Gtk::HBox.new(true, 0)
    vbox.pack_start(hbox, false, false, 0)
    hbox.show

    button = Gtk::Button.new("10% Larger")
    button.signal_connect('clicked'){ big_10 }
    button.show
    hbox.pack_start(button, false, true, 0)

    button = Gtk::Button.new("10% Smaller")
    button.signal_connect('clicked'){ small_10 }
    button.show
    hbox.pack_start(button, false, true, 0)
    window.show
  end

  def big_10
    @width = @width + (@im.rgb_width * 0.1)
    @height = @height + (@im.rgb_height * 0.1)
    resize(@width.to_i, @height.to_i)
  end

  def small_10
    @width = @width - (@im.rgb_width * 0.1)
    @height = @height - (@im.rgb_height * 0.1)
    resize(@width.to_i, @height.to_i)
  end

  def resize(width, height)
    if(width <= 0 || height <= 0)
      return
    end
    @pixbox.remove(@pixmap)

    @im.render(width, height)
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

Resize.new(ARGV.shift)
Gtk.main
