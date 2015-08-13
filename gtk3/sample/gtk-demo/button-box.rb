# Copyright (c) 2003-2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Button Boxes

The Button Box widgets are used to arrange buttons with padding.
=end
require "common"

module Demo
  class ButtonBox < BasicWindow
    def initialize
      super("Button Boxes")
      set_border_width(10)

      main_vbox = Gtk::Box.new(:vertical)
      add(main_vbox)

      frame_horiz = Gtk::Frame.new("Horizontal Button Boxes")
      main_vbox.pack_start(frame_horiz, :expand => true, :fill => true, :padding => 10)

      vbox = Gtk::Box.new(:vertical)
      vbox.set_border_width(10)
      frame_horiz.add(vbox)

      vbox.pack_start(create_bbox(true, "Spread", 40, :spread),
                      :expand => true, :fill => true, :padding => 0)

      vbox.pack_start(create_bbox(true, "Edge", 40, :edge),
                      :expand => true, :fill => true, :padding => 5)

      vbox.pack_start(create_bbox(true, "Start", 40, :start),
                      :expand => true, :fill => true, :padding => 5)

      vbox.pack_start(create_bbox(true, "End", 40, :end),
                      :expand => true, :fill => true, :padding => 5)

      frame_vert = Gtk::Frame.new("Vertical Button Boxes")
      main_vbox.pack_start(frame_vert, :expand => true, :fill => true, :padding => 10)

     hbox = Gtk::Box.new(:horizontal, 0)
      hbox.set_border_width(10)
      frame_vert.add(hbox)

      hbox.pack_start(create_bbox(false, "Spread", 30, :spread),
                      :expand => true, :fill => true, :padding => 0)

      hbox.pack_start(create_bbox(false, "Edge", 30, :edge),
                      :expand => true, :fill => true, :padding => 5)

      hbox.pack_start(create_bbox(false, "Start", 30, :start),
                      :expand => true, :fill => true, :padding => 5)

      hbox.pack_start(create_bbox(false, "End", 30, :end),
                      :expand => true, :fill => true, :padding => 5)
    end

    def create_bbox(horizontal, title, spacing, layout)
      frame = Gtk::Frame.new(title)

      bbox = if horizontal
               Gtk::ButtonBox.new(:horizontal)
             else
               Gtk::ButtonBox.new(:vertical)
             end

      bbox.set_border_width(5)
      frame.add(bbox)

      bbox.layout_style = layout
      bbox.set_spacing(spacing)

      %w(Ok Cancel Help).each do |name|
        button = Gtk::Button.new(:label => name)
        bbox.add(button)
      end

      frame
    end
  end
end
