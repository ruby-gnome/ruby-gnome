=begin

  tooltips.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: tooltips.rb,v 1.5 2003/02/01 16:46:23 mutoh Exp $

=end

require 'sample'

class TooltipsSample < SampleWindow
  def initialize
    super("Tooltips")
    set_default_size(200, -1)

    tooltips = Gtk::Tooltips.new

    button = Gtk::ToggleButton.new("button1")
    tooltips.set_tip(button,"This is button 1", "ContextHelp/buttons/1")

    button2 = Gtk::ToggleButton.new("button2")
    tooltips.set_tip(button2,
		     "This is button 2. This is also a really long tooltip which probably won't fit on a single line and will therefore need to be wrapped. Hopefully the wrapping will work correctly.",
		     "ContextHelp/buttons/2_long")
    box2 = Gtk::VBox.new(false, 10)
    box2.set_border_width(10)
    box2.pack_start(button, true, true, 0)
    box2.pack_start(button2, true, true, 0)

    separator = Gtk::HSeparator.new
    button4 = Gtk::Button.new("close")
    button4.signal_connect("clicked") do destroy end
    button4.set_flags(Gtk::Widget::CAN_DEFAULT)
    tooltips.set_tip(button4, "Push this button to close window", "ContextHelp/buttons/Close")
    box4 = Gtk::VBox.new(false, 10)
    box4.set_border_width(10)
    box4.pack_start(button4, true, true, 0)
    
    box1 = Gtk::VBox.new(false, 0)
    box1.pack_start(box2, true, true, 0)
    box1.pack_start(separator, false, true, 0)
    box1.pack_start(box4, false, true, 0)
    add(box1)
    button4.grab_default #This should be called after add().
  end
end
