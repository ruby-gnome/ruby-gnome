=begin

  tooltips.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: tooltips.rb,v 1.6 2005/07/21 17:47:19 mutoh Exp $

=end

require 'sample'

class TooltipsSample < SampleWindow
  def initialize
    super("Tooltips")
    set_default_size(200, -1)
    set_border_width(5)

    tooltips = Gtk::Tooltips.new

    button = Gtk::ToggleButton.new("button1")
    tooltips.set_tip(button,"This is button 1", "ContextHelp/buttons/1")

    button2 = Gtk::ToggleButton.new("button2")
    tooltips.set_tip(button2,
		     "This is button 2. This is also a really long tooltip which probably won't fit on a single line and will therefore need to be wrapped. Hopefully the wrapping will work correctly.",
		     "ContextHelp/buttons/2_long")
    vbox = Gtk::VBox.new(false, 10)
    vbox.add(button)
    vbox.add(button2)
    vbox.add(Gtk::HSeparator.new)

    button4 = Gtk::Button.new("close")
    button4.signal_connect("clicked"){destroy}
    vbox.pack_start(button4)

    tooltips.set_tip(button4, "Push this button to close window", "ContextHelp/buttons/Close")

    add(vbox)
  end
end
