=begin

  savedposition.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: savedposition.rb,v 1.6 2003/02/01 16:46:23 mutoh Exp $

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

    main_vbox = Gtk::VBox::new(false, 5)
    main_vbox.set_border_width(0)
    add(main_vbox)
    main_vbox.show

    vbox = Gtk::VBox::new(false, 5)
    vbox.set_border_width(10)
    main_vbox.pack_start(vbox, true, true, 0)
    vbox.show

    hbox = Gtk::HBox::new(false, 0)
    hbox.set_border_width(5)
    vbox.pack_start(hbox, false, true, 0)
    hbox.show

    label = Gtk::Label::new("X Origin : ")
    label.set_alignment(0, 0.5)
    hbox.pack_start(label, false, true, 0)
    label.show

    x_label = Gtk::Label::new("")
    hbox.pack_start(x_label, true, true, 0)
    x_label.show

    hbox = Gtk::HBox::new(false, 0)
    hbox.set_border_width(5)
    vbox.pack_start(hbox, false, true, 0)
    hbox.show

    label = Gtk::Label::new("Y Origin : ")
    label.set_alignment(0, 0.5)
    hbox.pack_start(label, false, true, 0)
    label.show

    y_label = Gtk::Label::new("")
    hbox.pack_start(y_label, true, true, 0)
    y_label.show

    signal_connect("configure_event") do
      x, y = window.root_origin
      Pos[0] = x
      Pos[1] = y
      x_label.set_text(x.to_s)
      y_label.set_text(y.to_s)
    end

    any = Gtk::HSeparator::new()
    main_vbox.pack_start(any, false, true, 0)
    any.show

    hbox = Gtk::HBox::new(false, 0)
    hbox.set_border_width(10)
    main_vbox.pack_start(hbox, false, true, 0);
    hbox.show

    button = Gtk::Button::new("Close")
    button.signal_connect("clicked") do
      destroy
    end
    hbox.pack_start(button, true, true, 5)
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default
    button.show
  end
end
