# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: colorsel.rb,v 1.4 2005/02/12 23:02:43 kzys Exp $
=begin
= Color Selector 

Gtk::ColorSelection lets the user choose a color. Gtk::ColorSelectionDialog
is a prebuilt dialog containing a Gtk::ColorSelection.
=end
require 'common'

module Demo
  class ColorSel < BasicWindow
    def initialize
      super('Color Selection')

      @color = Gdk::Color.new(0, 0, 65535)

      set_border_width(8)
      
      vbox = Gtk::VBox.new(false, 0)
      vbox.set_border_width(8)
      add(vbox)

      ## Create the color swatch area
      @frame = Gtk::Frame.new
      @frame.set_shadow_type(Gtk::SHADOW_IN)
      vbox.pack_start(@frame, true, true, 0)

      @da = Gtk::DrawingArea.new
      
      @da.signal_connect('expose_event') do |widget, event|
	if widget.window
	  style = widget.style

	  widget.window.draw_rectangle(style.bg_gc(Gtk::STATE_NORMAL),
				       true,
				       event.area.x, event.area.y,
				       event.area.width, event.area.height)
	end
      end

      # set a minimum size
      @da.set_size_request(200, 200)
      # set the color
      @da.modify_bg(Gtk::STATE_NORMAL, @color)

      @frame.add(@da)

      alignment = Gtk::Alignment.new(1.0, 0.5, 0.0, 0.0)

      button = Gtk::Button.new('_Change the above color', true)
      alignment.add(button)

      vbox.pack_start(alignment, false, false, 0)

      button.signal_connect('clicked') do
	change_color_callback
      end
    end

    def change_color_callback
      dialog = Gtk::ColorSelectionDialog.new('Changing color')

      dialog.set_transient_for(self)

      colorsel = dialog.colorsel

      colorsel.set_previous_color(@color)
      colorsel.set_current_color(@color)
      colorsel.set_has_palette(true)

      response = dialog.run
      
      if response == Gtk::Dialog::RESPONSE_OK
	@color = colorsel.current_color
	@da.modify_bg(Gtk::STATE_NORMAL, @color)
      end

      dialog.destroy
    end
  end
end
