=begin

  colorselect.rb - a part of testgtk.c rewritten in ruby-gtk

=end

require 'sample'

class ColorSelectionSample < Gtk::ColorSelectionDialog
  include Sample

  def initialize
    super("color selection dialog")
    @destroyed = false
    signal_connect("destroy") do destroy end

    colorsel.set_opacity(true)
    colorsel.set_update_policy(Gtk::UPDATE_CONTINUOUS)
    set_position(Gtk::WIN_POS_MOUSE)
    colorsel.signal_connect("color_changed") do selection_changed end
    ok_button.signal_connect("clicked") do selection_ok end
    cancel_button.signal_connect("clicked") do destroy end
  end

  def selection_ok
    color = colorsel.get_color
    colorsel.set_color(color)
  end

  def selection_changed
    color = colorsel.get_color
  end

end

class << ColorSelectionSample
  include SampleClass
end
