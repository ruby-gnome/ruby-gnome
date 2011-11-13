=begin header

  range.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: range.rb,v 1.6 2005/07/18 17:13:32 mutoh Exp $

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

    vbox = Gtk::VBox.new(false, 10)
    add(vbox)

    adjustment = Gtk::Adjustment.new(0.0, 0.0, 101.0, 0.1, 1.0, 1.0)

    scale = Gtk::HScale.new(adjustment)
    scale.set_size_request(150, 50)
    scale.update_policy = Gtk::UPDATE_DELAYED
    scale.digits = 1
    scale.draw_value = true

    vbox.add(scale)

    scrollbar = Gtk::HScrollbar.new(adjustment)
    scrollbar.update_policy = Gtk::UPDATE_CONTINUOUS
    vbox.add(scrollbar)

    vbox.add(Gtk::HSeparator.new)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){destroy}

    vbox.pack_start(button, true, true, 0)
    button.can_default = true
    button.grab_default
  end
end
