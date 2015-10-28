# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Scale

GtkScale is a way to select a value from a range.
Scales can have marks to help pick special values,
and they can also restrict the values that can be
chosen.
=end

module ScaleDemo
  def self.run_demo(main_window)
    builder = Gtk::Builder.new(:resource => "/scale/scale.ui")
    builder.connect_signals {}
    window = builder["window1"]
    window.screen = main_window.screen

    window.signal_connect("destroy") { window.destroyed(window) }

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
  end
end
