=begin header

  reparent.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: reparent.rb,v 1.5 2003/02/01 16:46:23 mutoh Exp $

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

class ReparentSample < SampleWindow
  def initialize
    super("reparent")

    box1 = Gtk::VBox::new(false, 0)
    add(box1)
    box1.show

    box2 = Gtk::HBox::new(false, 5)
    box2.set_border_width(10)
    box1.pack_start(box2, true, true, 0)
    box2.show

    label = Gtk::Label::new("Hello World")

    frame = Gtk::Frame::new("Frame 1")
    box2.pack_start(frame, true, true, 0)
    frame.show

    framebox1 = Gtk::VBox::new(false, 5)
    framebox1.set_border_width(5)
    frame.add(framebox1)
    framebox1.show

    button = Gtk::Button::new("switch")
    button.signal_connect("clicked") do
      label.reparent(framebox1)
    end
    framebox1.pack_start(button, false, true, 0)
    button.show

    framebox1.pack_start(label, false, true, 0)
    label.signal_connect("parent_set") do |child, old_parent|
      print("set_parent for \"#{child.class}\":")
      print(" new parent: \"#{if child.parent then child.parent.class else 'NULL' end}\",")
      print(" old parent: \"#{if old_parent then old_parent.class else 'NULL' end}\",")
      print(" data: 42\n")
    end
    label.show

    frame = Gtk::Frame::new("Frame 2")
    box2.pack_start(frame, true, true, 0)
    frame.show

    framebox2 = Gtk::VBox::new(false, 5)
    framebox2.set_border_width(5)
    frame.add(framebox2)
    framebox2.show

    button = Gtk::Button::new("switch")
    button.signal_connect("clicked") do
      label.reparent(framebox2)
    end
    framebox2.pack_start(button, false, true, 0)
    button.show

    separator = Gtk::HSeparator::new()
    box1.pack_start(separator, false, true, 0)
    separator.show

    box2 = Gtk::VBox::new(false, 10)
    box2.set_border_width(10)
    box1.pack_start(box2, false, true, 0)
    box2.show

    button = Gtk::Button::new("close")
    button.signal_connect("clicked") do
      destroy
    end
    box2.pack_start(button, true, true, 0)
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default
    button.show
  end
end
