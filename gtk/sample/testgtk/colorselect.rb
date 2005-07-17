=begin

  colorselect.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project
  $Id: colorselect.rb,v 1.5 2005/07/17 16:55:27 mutoh Exp $
=end

require 'sample'

class ColorSelectionSample < Gtk::ColorSelectionDialog
  include Sample
  extend SampleClass

  def initialize
    super("color selection dialog")
    @destroyed = false
    signal_connect("destroy"){destroy}

    colorsel.set_has_opacity_control(true)
    set_window_position(Gtk::Window::POS_MOUSE)

    colorsel.signal_connect("color_changed"){
      colorsel.current_color
    }
    ok_button.signal_connect("clicked"){
      color = colorsel.current_color
      colorsel.set_current_color(color)
    }
    cancel_button.signal_connect("clicked"){destroy}
  end
end

