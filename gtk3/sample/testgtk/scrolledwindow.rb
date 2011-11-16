=begin header

  scrolledwindow.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2005/07/21 17:47:19 $
  $Id: scrolledwindow.rb,v 1.7 2005/07/21 17:47:19 mutoh Exp $

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

class ScrolledWindowSample < SampleDialog
  def initialize
    super("dialog")

    scrolled_window = Gtk::ScrolledWindow.new
    scrolled_window.border_width = 10
    scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC,
                               Gtk::POLICY_AUTOMATIC)
    vbox.add(scrolled_window)

    table = Gtk::Table.new(20, 20, false)
    table.row_spacings = 10
    table.column_spacings = 10
    scrolled_window.add_with_viewport(table)

    table.focus_hadjustment = scrolled_window.hadjustment
    table.focus_vadjustment = scrolled_window.vadjustment

    for i in 0..19
      for j in 0..19
        buffer = "button (#{i},#{j})"
        button = Gtk::ToggleButton.new(buffer)
        table.attach(button, i, i + 1, j, j + 1)
      end
    end

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){destroy}
    action_area.pack_start(button, true, true, 0)

    set_default_size(300, 300)
  end
end
