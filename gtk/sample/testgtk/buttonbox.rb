=begin header

  buttonbox.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  $Id: buttonbox.rb,v 1.5 2003/02/01 16:46:23 mutoh Exp $

  Rewritten by Minoru Inachi <inachi@earth.interq.or.jp>

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

class ButtonBoxSample < SampleWindow
  def initialize
    super("Button Boxes")
    signal_connect("destroy") do destroy end
    self.border_width = 10

    main_vbox = Gtk::VBox.new
    add(main_vbox)
    
    frame_horz = Gtk::Frame.new("Horizontal Button Boxes")
    main_vbox.pack_start(frame_horz, true, true, 10)
    
    vbox = Gtk::VBox.new
    vbox.border_width = 10
    frame_horz.add(vbox)

    vbox.pack_start(create_bbox(true, "Spread", 40,
				Gtk::ButtonBox::SPREAD))

    vbox.pack_start(create_bbox(true, "Edge", 40,
				Gtk::ButtonBox::EDGE), true, true, 5)

    vbox.pack_start(create_bbox(true, "Start", 40,
				Gtk::ButtonBox::START), true, true, 5)

    vbox.pack_start(create_bbox(true, "End", 40,
				Gtk::ButtonBox::END), true, true, 5)

    frame_vert = Gtk::Frame.new("Vertical Button Boxes")
    main_vbox.pack_start(frame_vert, true, true, 10)

    hbox = Gtk::HBox.new
    hbox.border_width = 10
    frame_vert.add(hbox)

    hbox.pack_start(create_bbox(false, "Spread", 30,
				Gtk::ButtonBox::SPREAD))

    hbox.pack_start(create_bbox(false, "Edge", 30,
				Gtk::ButtonBox::EDGE), true, true, 5)

    hbox.pack_start(create_bbox(false, "Start", 30,
				Gtk::ButtonBox::START), true, true, 5)

    hbox.pack_start(create_bbox(false, "End", 30,
				Gtk::ButtonBox::END), true, true, 5)
  end

  def create_bbox(horizontal, title, spacing, layout)
    frame = Gtk::Frame.new(title)

    if horizontal then
      bbox = Gtk::HButtonBox.new
    else
      bbox = Gtk::VButtonBox.new
    end

    bbox.border_width = 5
    frame.add(bbox)

    bbox.set_layout_style(layout)
    bbox.set_spacing(spacing)

    button = Gtk::Button.new("OK")
    bbox.add(button)
  
    button = Gtk::Button.new("Cancel")
    bbox.add(button)
 
    button = Gtk::Button.new("Help")
    bbox.add(button)

    frame
  end
end
