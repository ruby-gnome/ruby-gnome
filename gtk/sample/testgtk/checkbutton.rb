=begin header

  checkbutton.rb - a part of testgtk.c rewritten in ruby-gtk

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2002/05/19 12:39:14 $
  $Id: checkbutton.rb,v 1.1 2002/05/19 12:39:14 mutoh Exp $

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

    box1 = Gtk::VBox::new(false, 0)
    add(box1)
    box1.show

    box2 = Gtk::VBox::new(false, 10)
    box2.border_width(10)
    box1.pack_start(box2, true, true, 0)
    box2.show

    button = Gtk::CheckButton::new("button1")
    box2.pack_start(button, true, true, 0)
    button.show

    button = Gtk::CheckButton::new("button2")
    box2.pack_start(button, true, true, 0)
    button.show

    button = Gtk::CheckButton::new("button3")
    box2.pack_start(button, true, true, 0)
    button.show

    separator = Gtk::HSeparator::new()
    box1.pack_start(separator, false, true, 0)
    separator.show

    box2 = Gtk::VBox::new(false, 10)
    box2.border_width(10)
    box1.pack_start(box2, false, true, 0)
    box2.show

    button = Gtk::Button::new("close")
    button.signal_connect("clicked") do destroy end

    box2.pack_start(button, true, true, 0)
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default
    button.show
  end
end
