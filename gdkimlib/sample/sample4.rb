#!/usr/local/bin/ruby

require 'gtk'
require 'gdk_imlib'

class Modifier
  def initialize(filename)
    @im = nil
    @pixmap = nil
    @pixbox = nil

    window = Gtk::Window.new(Gtk::WINDOW_TOPLEVEL)
    window.set_title(filename)
    window.border_width = 0
    window.signal_connect('destroy'){ exit }
    window.signal_connect('delete_event') { exit }

    mainbox = Gtk::VBox.new(false, 0)
    window.add(mainbox)

    @im = GdkImlib::Image.new(filename)
    @im.render(@im.rgb_width, @im.rgb_height)
    pixd, maskd = @im.get_pixmap
    @pixmap = Gtk::Pixmap.new(pixd, maskd)

    modifier = @im.modifier

    @pixbox = Gtk::VBox.new(false, 0)
    @pixbox.pack_start(@pixmap, false, false, 0)
    mainbox.pack_start(@pixbox)

    modbox = Gtk::VBox.new(false, 0)

    box = Gtk::VBox.new(false, 0)
    box.pack_start(Gtk::Label.new("brightness"))
    adj = Gtk::Adjustment.new(modifier.brightness, 0.0, 513.0, 1.0, 10.0, 1.0)
    adj.signal_connect('value_changed'){|widget|
      change_brightness(widget.value)
    }
    scale = Gtk::HScale.new(adj)
    scale.set_usize(200, 30)
    box.pack_start(scale, true, true, 0)
    modbox.pack_start(box, false, false, 0)

    box = Gtk::VBox.new(false, 0)
    box.pack_start(Gtk::Label.new("contrast"))
    adj = Gtk::Adjustment.new(modifier.contrast, 0.0, 513.0, 1.0, 10.0, 1.0)
    adj.signal_connect('value_changed'){|widget|
      change_contrast(widget.value)
    }
    scale = Gtk::HScale.new(adj)
    scale.set_usize(200, 30)
    box.pack_start(scale, true, true, 0)
    modbox.pack_start(box, false, false, 0)

    box = Gtk::VBox.new(false, 0)
    box.pack_start(Gtk::Label.new("gamma"))
    adj = Gtk::Adjustment.new(modifier.gamma, 0.0, 513.0, 1.0, 10.0, 1.0)
    adj.signal_connect('value_changed'){|widget|
      change_gamma(widget.value)
    }
    scale = Gtk::HScale.new(adj)
    scale.set_usize(200, 30)
    box.pack_start(scale, true, true, 0)
    modbox.pack_start(box, false, false, 0)

    mainbox.pack_start(modbox)

    window.show_all
  end

  def change_brightness(value)
    mod = @im.modifier
    mod.brightness = value.to_i
    @im.modifier = mod
    rerender
  end

  def change_contrast(value)
    mod = @im.modifier
    mod.contrast = value.to_i
    @im.modifier = mod
    rerender
  end

  def change_gamma(value)
    mod = @im.modifier
    mod.gamma = value.to_i
    @im.modifier = mod
    rerender
  end

  def rerender
    @pixbox.remove(@pixmap)
    @im.render(@im.rgb_width, @im.rgb_height)
    pixd, maskd = @im.get_pixmap
    @pixmap = Gtk::Pixmap.new(pixd, maskd)
    @pixmap.show
    @pixbox.pack_start(@pixmap, false, false, 0)
#    GC.start
  end
end

if(ARGV.size != 1)
  print $0, " filename\n"
  exit
end

GdkImlib::Image.render_limit = 4000000

Modifier.new(ARGV.shift)

Gtk.main
