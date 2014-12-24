=begin header

  radiobutton.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2005/07/18 17:13:32 $
  $Id: radiobutton.rb,v 1.7 2005/07/18 17:13:32 mutoh Exp $

Original Copyright:
 
  GTK - The GIMP Toolkit
  Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
 
  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.

=end

require 'sample'

class RadioButtonSample < SampleWindow
  def initialize
    super("radio buttons")
    set_border_width(10)

    vbox = Gtk::VBox.new(false, 10)
    add(vbox)

    button1 = Gtk::RadioButton.new("button1")
    vbox.add(button1)

    button2 = Gtk::RadioButton.new(button1, "button2")
    vbox.add(button2)

    button3 = Gtk::RadioButton.new(button1, "button3")
    vbox.add(button3)

    vbox.add(Gtk::HSeparator.new)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){destroy}
    vbox.pack_start(button, false, true)

    button.can_default = true
    button.grab_default
  end
end
