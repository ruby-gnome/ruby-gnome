=begin

  colorselect.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project
  $Id: colorselect.rb,v 1.4 2003/01/19 14:28:24 mutoh Exp $
=end

require 'sample'

class ColorSelectionSample < Gtk::ColorSelectionDialog
  include Sample

  def initialize
    super("color selection dialog")
    @destroyed = false
    signal_connect("destroy") do destroy end

    colorsel.set_has_opacity_control(true)
    set_window_position(Gtk::Window::POS_MOUSE)
    colorsel.signal_connect("color_changed") do selection_changed end
    ok_button.signal_connect("clicked") do selection_ok end
    cancel_button.signal_connect("clicked") do destroy end
  end

  def selection_ok
    color = colorsel.current_color
    colorsel.set_current_color(color)
  end

  def selection_changed
    color = colorsel.current_color
  end

end

class << ColorSelectionSample
  include SampleClass
end
