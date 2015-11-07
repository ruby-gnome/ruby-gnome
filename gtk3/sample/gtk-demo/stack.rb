# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Stack

GtkStack is a container that shows a single child at a time,
with nice transitions when the visible child changes.

GtkStackSwitcher adds buttons to control which child is visible.
=end

module StackDemo
  def self.run_demo(main_window)
    builder = Gtk::Builder.new(:resource => "/stack/stack.ui")
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
