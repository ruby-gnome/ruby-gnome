=begin header

  wmhints.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: wmhints.rb,v 1.7 2003/02/01 16:46:23 mutoh Exp $
=end

require 'sample'

unless defined? Gdk::Pixmap::create_from_xbm()
  class << Gdk::Pixmap
    def create_from_xbm(window, fname)
      open(fname) do |fh|
	while line = fh.gets
	  width = $1.to_i if /^#define\s+\w+_width\s+(\d+)/ === line
	  height = $1.to_i if /^#define\s+\w+_height\s+(\d+)/ === line
	  if /^static\s+(?:unsigned)?\s*char\s+\w+\[\]\s*=\s*\{/ === line
	    raise "invalid width or height" if not width or not height
	    break
	  end
	end
	bits = [fh.read.gsub(/0[xX]/, "").delete("^0-9A-Fa-f")].pack("H*")
	raise "invalid format" if bits == ""
	create_from_data(window, bits, width, height)
      end
    end
  end
end

class WMHintsSample < SampleWindow
  def initialize
    super("WM Hints")
    set_border_width(0)
    realize

    circles = Gdk::Pixmap::create_from_xbm(window, "circles.xbm")
    window.set_icon(nil, circles, circles)
    window.set_icon_name("WMHints Test Icon")
    window.set_decorations(Gdk::Window::DECOR_ALL | Gdk::Window::DECOR_MENU)
    window.set_functions(Gdk::Window::FUNC_ALL | Gdk::Window::FUNC_RESIZE)

    box1 = Gtk::VBox.new(false, 0)
    add(box1)

    label = Gtk::Label.new("Try iconizing me!")
    label.set_size_request(150, 50)
    box1.pack_start(label, true, true, 0)

    separator = Gtk::HSeparator.new()
    box1.pack_start(separator, false, true, 0)

    box2 = Gtk::VBox.new(false, 10)
    box2.set_border_width(10)
    box1.pack_start(box2, false, true, 0)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked") do
      destroy
    end

    box2.pack_start(button, true, true, 0)
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default
  end
end
