=begin

  savedposition.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: savedposition.rb,v 1.7 2005/07/21 17:47:19 mutoh Exp $

  Rewritten by TAKAHASHI Hitoshi <thitoshi@ne.scphys.kyoto-u.ac.jp>

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

class SavedPositionSample < SampleWindow
  Pos = [ 0, 0 ]

  def initialize
    super("Saved Position")
    move(Pos[0], Pos[1])
    set_default_size(200, 100)
    set_border_width = 5

    vbox = Gtk::VBox.new
    add(vbox)

    hbox1 = Gtk::HBox.new
    hbox1.border_width = 5
    vbox.add(hbox1)

    label = Gtk::Label.new("X Origin : ")
    label.set_alignment(0, 0.5)
    hbox1.add(label)

    x_label = Gtk::Label.new("")
    hbox1.add(x_label)

    hbox2 = Gtk::HBox.new
    hbox2.set_border_width(5)
    vbox.add(hbox2)

    label = Gtk::Label.new("Y Origin : ")
    label.set_alignment(0, 0.5)
    hbox2.add(label)
 
    y_label = Gtk::Label.new("")
    hbox2.add(y_label)

    signal_connect("configure_event") do
      x, y = window.root_origin
      Pos[0] = x
      Pos[1] = y
      x_label.text = x.to_s
      y_label.text = y.to_s
      false
    end

    vbox.pack_start(Gtk::HSeparator.new, false, true, 10)

    button = Gtk::Button.new("Close")
    button.signal_connect("clicked"){destroy}

    vbox.add(button)
    button.can_default = true
    button.grab_default
  end
end
