=begin header

  checkbutton.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
  $Id: checkbutton.rb,v 1.6 2005/07/17 16:55:26 mutoh Exp $

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2005/07/17 16:55:26 $
  $Id: checkbutton.rb,v 1.6 2005/07/17 16:55:26 mutoh Exp $

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

class CheckButtonSample < SampleWindow
  def initialize
    super("check buttons")

    box1 = Gtk::VBox.new
    add(box1)

    box2 = Gtk::VBox.new(false, 10)
    box2.border_width = 10
    box1.add(box2)

    button = Gtk::CheckButton.new("button1")
    box2.add(button)

    button = Gtk::CheckButton.new("button2")
    box2.add(button)

    button = Gtk::CheckButton.new("button3")
    box2.add(button)

    box1.pack_start(Gtk::HSeparator.new)

    box2 = Gtk::VBox.new(false, 10)
    box2.border_width = 10
    box1.add(box2)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){destroy}

    box2.add(button)
  end
end
