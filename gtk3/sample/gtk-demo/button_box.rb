# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: button_box.rb,v 1.3 2005/02/12 23:02:43 kzys Exp $
=begin
= Button Boxes

The Button Box widgets are used to arrange buttons with padding.
=end
require 'common'

module Demo
  class ButtonBox < BasicWindow
    def initialize
      super('Button Boxes')
      set_border_width(10)

      main_vbox = Gtk::VBox.new(false, 0)
      add(main_vbox)

      frame_horiz = Gtk::Frame.new('Horizontal Button Boxes')
      main_vbox.pack_start(frame_horiz, true, true, 10)

      vbox = Gtk::VBox.new(false, 0)
      vbox.set_border_width(10)
      frame_horiz.add(vbox)

      vbox.pack_start(create_bbox(true, 'Spread', 40, Gtk::ButtonBox::SPREAD),
		      true, true, 0)
      
      vbox.pack_start(create_bbox(true, 'Edge', 40, Gtk::ButtonBox::EDGE),
		      true, true, 5)
      
      vbox.pack_start(create_bbox(true, 'Start', 40, Gtk::ButtonBox::START),
		      true, true, 5)
      
      vbox.pack_start(create_bbox(true, 'End', 40, Gtk::ButtonBox::END),
		      true, true, 5)
      
      frame_vert = Gtk::Frame.new('Vertical Button Boxes')
      main_vbox.pack_start(frame_vert, true, true, 10)

      hbox = Gtk::HBox.new(false, 0)
      hbox.set_border_width(10)
      frame_vert.add(hbox)

      hbox.pack_start(create_bbox(false, 'Spread', 30, Gtk::ButtonBox::SPREAD),
		      true, true, 0)
      
      hbox.pack_start(create_bbox(false, 'Edge', 30, Gtk::ButtonBox::EDGE),
		      true, true, 5)
      
      hbox.pack_start(create_bbox(false, 'Start', 30, Gtk::ButtonBox::START),
		      true, true, 5)
      
      hbox.pack_start(create_bbox(false, 'End', 30, Gtk::ButtonBox::END),
		      true, true, 5)
      
    end

    def create_bbox(horizontal, title, spacing, layout)
      frame = Gtk::Frame.new(title)
      
      bbox = if horizontal
	       Gtk::HButtonBox.new
	     else
	       Gtk::VButtonBox.new
	     end

      bbox.set_border_width(5)
      frame.add(bbox)

      bbox.layout_style = layout
      bbox.set_spacing(spacing)

      [Gtk::Stock::OK, Gtk::Stock::CANCEL, Gtk::Stock::HELP].each do |stock|
	button = Gtk::Button.new(stock)
	bbox.add(button)
      end

      return frame
    end
  end
end
