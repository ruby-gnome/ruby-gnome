=begin header

  togglebutton.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2005/07/21 17:47:19 $
  $Id: togglebutton.rb,v 1.6 2005/07/21 17:47:19 mutoh Exp $

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

class ToggleButtonSample < SampleWindow
  def initialize
    super("toggle buttons")

    set_border_width(5)

    vbox = Gtk::VBox.new(false, 5)
    add(vbox)

    (1..3).each do |i|
      vbox.add(Gtk::ToggleButton.new("button#{i}"))
    end
    vbox.pack_start(Gtk::HSeparator.new, false, true, 0)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){destroy}
    vbox.add(button)
  end
end
