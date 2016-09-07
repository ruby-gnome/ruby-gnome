# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Scale

GtkScale is a way to select a value from a range.
Scales can have marks to help pick special values,
and they can also restrict the values that can be
chosen.
=end

class ScaleDemo
  def initialize(main_window)
    builder = Gtk::Builder.new(:resource => "/scale/scale.ui")
    builder.connect_signals {}
    @window = builder["window1"]
    @window.screen = main_window.screen
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
  end
end
