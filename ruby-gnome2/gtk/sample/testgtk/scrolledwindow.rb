=begin header

  scrolledwindow.rb - a part of testgtk.c rewritten in ruby-gtk

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2002/05/19 12:29:43 $
  $Id: scrolledwindow.rb,v 1.1 2002/05/19 12:29:43 mutoh Exp $

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

    scrolled_window = Gtk::ScrolledWindow::new(nil, nil)
    scrolled_window.border_width(10)
    scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC,
                               Gtk::POLICY_AUTOMATIC)
    vbox.pack_start(scrolled_window, true, true, 0)
    scrolled_window.show

    table = Gtk::Table::new(20, 20, false)
    table.set_row_spacings(10)
    table.set_col_spacings(10)
    scrolled_window.add_with_viewport(table)
    table.set_focus_hadjustment(scrolled_window.get_hadjustment)
    table.set_focus_vadjustment(scrolled_window.get_vadjustment)
    table.show

    for i in 0..19
      for j in 0..19
        buffer = sprintf("button (%d,%d)\n", i, j)
        button = Gtk::ToggleButton::new(buffer)
        table.attach(button, i, i+1, j, j+1)
        button.show
      end
    end

    button = Gtk::Button::new("close")
    button.signal_connect("clicked") do destroy end
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    action_area.pack_start(button, true, true, 0)
    button.grab_default
    button.show

    set_default_size(300, 300)
  end
end
