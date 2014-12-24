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

      @color = Gdk::RGBA.new(0, 0, 1, 1)

      set_border_width(8)

      vbox = Gtk::Box.new(:vertical, 0)
      vbox.set_border_width(8)
      add(vbox)

      ## Create the color swatch area
      @frame = Gtk::Frame.new
      @frame.set_shadow_type(:in)
      vbox.pack_start(@frame, :expand => true, :fill => true, :padding => 0)

      @da = Gtk::DrawingArea.new

      @da.signal_connect('draw') do |widget, event|
        if widget.window
          context = widget.style_context
          background_color = context.get_background_color(:normal)
          event.set_source_rgba(background_color.to_a)
          event.paint
        end
      end

      # set a minimum size
      @da.set_size_request(200, 200)
      # set the color
      @da.override_background_color(:normal, @color)

      @frame.add(@da)

      alignment = Gtk::Alignment.new(1.0, 0.5, 0.0, 0.0)

      button = Gtk::Button.new(:mnemonic => '_Change the above color')
      alignment.add(button)

      vbox.pack_start(alignment, :expand => false, :fill => false, :padding => 0)

      button.signal_connect('clicked') do
        change_color_callback
      end
    end

    def change_color_callback
      dialog = Gtk::ColorSelectionDialog.new(:title => 'Changing color')

      dialog.set_transient_for(self)

      colorsel = dialog.color_selection

      colorsel.set_previous_rgba(@color)
      colorsel.set_current_rgba(@color)
      colorsel.set_has_palette(true)

      response = dialog.run

      if response == Gtk::ResponseType::OK
        @color = colorsel.current_rgba
        @da.override_background_color(:normal, @color)
      end

      dialog.destroy
    end
  end
end
