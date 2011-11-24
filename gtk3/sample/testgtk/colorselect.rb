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
    super(:title => "color selection dialog")
    @destroyed = false
    signal_connect("destroy"){destroy}

    color_selection.set_has_opacity_control(true)
    set_window_position(:mouse) # Gtk::Window::Position::MOUSE

    color_selection.signal_connect("color_changed"){
      color_selection.current_color
    }
    ok_button.signal_connect("clicked"){
      puts color_selection.current_color
      destroy
    }
    cancel_button.signal_connect("clicked"){destroy}
  end
end

