=begin header

  pixmap.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2005/07/18 17:13:32 $
  $Id: pixmap.rb,v 1.5 2005/07/18 17:13:32 mutoh Exp $

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

class PixmapSample < SampleWindow
  def initialize
    super("pixmap")

    vbox = Gtk::VBox.new(false, 0)
    add(vbox)

    button = Gtk::Button.new
    vbox.pack_start(button, false, false, 10)

    label = Gtk::Label.new("Pixmap\ntest")
    boxbutton = Gtk::HBox.new(false, 0)
    boxbutton.border_width = 2
    boxbutton.add(Gtk::Image.new("test.xpm"))
    boxbutton.add(label)
    button.add(boxbutton)

    vbox.add(Gtk::HSeparator.new)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){destroy}
    vbox.pack_start(button, false, false, 10)
  end
end
