=begin header

  reparent.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: reparent.rb,v 1.6 2005/07/18 17:13:33 mutoh Exp $

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

    vbox = Gtk::VBox.new(false, 0)
    add(vbox)

    box2 = Gtk::HBox.new(false, 5)
    box2.set_border_width(10)
    vbox.pack_start(box2, true, true, 0)

    label = Gtk::Label.new("Hello World")

    frame = Gtk::Frame.new("Frame 1")
    box2.pack_start(frame, true, true, 0)

    framebox1 = Gtk::VBox.new(false, 5)
    framebox1.border_width = 5
    frame.add(framebox1)

    button = Gtk::Button.new("switch")
    button.signal_connect("clicked"){
      label.reparent(framebox1)
    }
    framebox1.pack_start(button, false, true, 0)

    framebox1.add(label)
    label.signal_connect("parent_set"){ |child, old_parent|
      puts "set_parent for \"#{child.class}\":"
      puts " new parent: \"#{if child.parent then child.parent.class else 'NULL' end}\","
      puts " old parent: \"#{if old_parent then old_parent.class else 'NULL' end}\","
      puts " data: 42\n"
    }

    frame = Gtk::Frame.new("Frame 2")
    box2.pack_start(frame, true, true, 0)

    framebox2 = Gtk::VBox.new(false, 5)
    framebox2.border_width = 5
    frame.add(framebox2)

    button = Gtk::Button.new("switch")
    button.signal_connect("clicked"){
      label.reparent(framebox2)
    }
    framebox2.pack_start(button, false, true, 0)

    vbox.pack_start(Gtk::HSeparator.new, false, true)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){destroy}

    vbox.pack_start(button, false, true, 10)
    button.can_default = true
    button.grab_default
  end
end
