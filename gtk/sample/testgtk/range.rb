=begin header

  range.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: range.rb,v 1.5 2003/02/01 16:46:23 mutoh Exp $

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

class RangeSample < SampleWindow
  def initialize
    super("range controls")

    box1 = Gtk::VBox::new(false, 0)
    add(box1)
    box1.show

    box2 = Gtk::VBox::new(false, 10)
    box2.set_border_width(10)
    box1.pack_start(box2, true, true, 0)
    box2.show

    adjustment = Gtk::Adjustment::new(0.0, 0.0, 101.0, 0.1, 1.0, 1.0)

    scale = Gtk::HScale::new(adjustment)
    scale.set_size_request(150, 50)
    scale.set_update_policy(Gtk::UPDATE_DELAYED)
    scale.set_digits(1)
    scale.set_draw_value(true)
    box2.pack_start(scale, true, true, 0)
    scale.show

    scrollbar = Gtk::HScrollbar::new(adjustment)
    scrollbar.set_update_policy(Gtk::UPDATE_CONTINUOUS)
    box2.pack_start(scrollbar, true, true, 0)
    scrollbar.show

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
