=begin header

  filesel.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: filesel.rb,v 1.5 2003/02/01 16:46:23 mutoh Exp $

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

class FileSelectionSample < Gtk::FileSelection
  include Sample
  extend SampleClass

  def initialize
    super("file selection dialog")
    @destroyed = false
    signal_connect("destroy") do destroy end

    set_window_position(Gtk::Window::POS_MOUSE)
    hide_fileop_buttons

    ok_button.signal_connect("clicked") do
      puts filename
      destroy
    end
    cancel_button.signal_connect("clicked") do
      destroy
    end

    button = Gtk::Button::new("Hide Fileops")
    button.signal_connect("clicked") do
      hide_fileop_buttons
    end
    action_area.pack_start(button, false, false, 0)

    button = Gtk::Button::new("Show Fileops")
    button.signal_connect("clicked") do
      show_fileop_buttons
    end
    action_area.pack_start(button, false, false, 0)
  end
end
