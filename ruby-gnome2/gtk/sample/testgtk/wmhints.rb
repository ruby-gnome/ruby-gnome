=begin header

  wmhints.rb - a part of testgtk.c rewritten in ruby-gtk

=end

require 'sample'

unless defined? Gdk::Bitmap::create_from_xbm()
  class << Gdk::Bitmap
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
    border_width(0)
    realize

    circles = Gdk::Bitmap::create_from_xbm(window, "circles.xbm")
    window.set_icon(nil, circles, circles)
    window.set_icon_name("WMHints Test Icon")
    window.set_decorations(Gdk::DECOR_ALL | Gdk::DECOR_MENU)
    window.set_functions(Gdk::FUNC_ALL | Gdk::FUNC_RESIZE)

    box1 = Gtk::VBox::new(false, 0)
    add(box1)
    box1.show

    label = Gtk::Label::new("Try iconizing me!")
    label.set_usize(150, 50)
    box1.pack_start(label, true, true, 0)
    label.show

    separator = Gtk::HSeparator::new()
    box1.pack_start(separator, false, true, 0)
    separator.show

    box2 = Gtk::VBox::new(false, 10)
    box2.border_width(10)
    box1.pack_start(box2, false, true, 0)
    box2.show

    button = Gtk::Button::new("close")
    button.signal_connect("clicked") do
      destroy
    end

    box2.pack_start(button, true, true, 0)
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default
    button.show
  end
end
