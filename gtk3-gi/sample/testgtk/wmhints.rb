=begin header

  wmhints.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: wmhints.rb,v 1.8 2005/07/21 17:47:19 mutoh Exp $
=end

require 'sample'

class WMHintsSample < SampleWindow
  def initialize
    super("WM Hints")

    realize

    circles = Gdk::Pixmap.create_from_xbm(window, "circles.xbm")
    window.set_icon(nil, circles, circles)
    window.icon_name = "WMHints Test Icon"
    window.decorations = Gdk::Window::DECOR_ALL | Gdk::Window::DECOR_MENU
    window.functions = Gdk::Window::FUNC_ALL | Gdk::Window::FUNC_RESIZE

    vbox = Gtk::VBox.new(false, 10)
    add(vbox)

    label = Gtk::Label.new("Try iconizing me!")
    label.set_size_request(150, 50)
    vbox.pack_start(label, true, true, 0)

    vbox.pack_start(Gtk::HSeparator.new, false, true, 0)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){destroy}

    vbox.pack_start(button, true, true, 0)
    button.can_default = true
    button.grab_default
  end
end
