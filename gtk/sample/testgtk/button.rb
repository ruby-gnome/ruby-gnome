=begin header

  button.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/05/04 03:49:58 $
  $Id: button.rb,v 1.5 2003/05/04 03:49:58 mutoh Exp $

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

class ButtonSample < SampleWindow
  def initialize
    super("buttons")

    box1 = Gtk::VBox::new(false, 0)
    add(box1)

    table = Gtk::Table::new(3, 3, false)
    table.set_row_spacings(5)
    table.set_column_spacings(5)
    table.set_border_width(10)
    box1.pack_start(table, true, true, 0)

    button = [    
      Gtk::Button::new("button1"),
      Gtk::Button::new("button2"),
      Gtk::Button::new("button3"),
      Gtk::Button::new("button4"),
      Gtk::Button::new("button5"),
      Gtk::Button::new("button6"),
      Gtk::Button::new("button7"),
      Gtk::Button::new("button8"),
      Gtk::Button::new("button9"),
    ]

    button[0].signal_connect("clicked") do
      button_window(button[1])
    end
    table.attach(button[0], 0, 1, 0, 1,
		 Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)

    button[1].signal_connect("clicked") do
      button_window(button[2])
    end
    table.attach(button[1], 1, 2, 1, 2,
		 Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)

    button[2].signal_connect("clicked") do
      button_window(button[3])
    end
    table.attach(button[2], 2, 3, 2, 3,
		 Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)

    button[3].signal_connect("clicked") do
      button_window(button[4])
    end
    table.attach(button[3], 0, 1, 2, 3,
		 Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)

    button[4].signal_connect("clicked") do
      button_window(button[5])
    end
    table.attach(button[4], 2, 3, 0, 1,
		 Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)

    button[5].signal_connect("clicked") do
      button_window(button[6])
    end
    table.attach(button[5], 1, 2, 2, 3,
		 Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)

    button[6].signal_connect("clicked") do
      button_window(button[7])
    end
    table.attach(button[6], 1, 2, 0, 1,
                 Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)

    button[7].signal_connect("clicked") do
      button_window(button[8])
    end
    table.attach(button[7], 2, 3, 1, 2,
		 Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)

    button[8].signal_connect("clicked") do
      button_window(button[0])
    end
    table.attach(button[8], 0, 1, 1, 2,
		 Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)

    separator = Gtk::HSeparator::new()
    box1.pack_start(separator, false, true, 0)

    box2 = Gtk::VBox::new(false, 10)
    box2.set_border_width(10)
    box1.pack_start(box2, false, true, 0)

    button[9] = Gtk::Button::new("close")
    button[9].signal_connect("clicked") do destroy end
    box2.pack_start(button[9], true, true, 0)
    button[9].set_flags(Gtk::Widget::CAN_DEFAULT)
    button[9].grab_default

  end

  private
  def button_window(button)
    unless button.visible?
      button.show
    else
      button.hide
    end
  end
end
