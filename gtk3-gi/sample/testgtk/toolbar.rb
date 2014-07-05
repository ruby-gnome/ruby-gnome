=begin header

  toolbar.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Id: toolbar.rb,v 1.7 2005/07/21 17:47:19 mutoh Exp $

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

class ToolbarSample < SampleWindow
  def initialize
    super("Toolbar test")

    toolbar = Gtk::Toolbar.new

    toolbar.append("Horizontal", "Horizontal toolbar layout",
		   "Toolbar/Horizontal", Gtk::Image.new("test.xpm")){
      toolbar.orientation = Gtk::ORIENTATION_HORIZONTAL
    }
    toolbar.append("Vertical", "Vertical toolbar layout",
		   "Toolbar/Vertical", Gtk::Image.new("test.xpm")){
      toolbar.orientation = Gtk::ORIENTATION_VERTICAL
    }

    toolbar.append_space

    toolbar.append("Icons", "Only show toolbar icons",
		   "Toolbar/IconsOnly", Gtk::Image.new("test.xpm")){
      toolbar.toolbar_style = Gtk::Toolbar::ICONS
    }
    toolbar.append("Text", "Only show toolbar text", "Toolbar/TextOnly",
		   Gtk::Image.new("test.xpm")){
      toolbar.toolbar_style = Gtk::Toolbar::TEXT
    }
    toolbar.append("Both", "Show toolbar icons and text", "Toolbar/Both",
                   Gtk::Image.new("test.xpm")){
      toolbar.toolbar_style = Gtk::Toolbar::BOTH
    }

    toolbar.append_space

    toolbar.append(Gtk::Entry.new, "This is an unusable Gtk::Entry ;)",
		   "Hey don't click me!!!")

    toolbar.append_space

    toolbar.append("Enable", "Enable tooltips", nil,
		   Gtk::Image.new("test.xpm")){
      toolbar.tooltips = true
    }
    toolbar.append("Disable", "Disable tooltips", nil,
		   Gtk::Image.new("test.xpm")){
      toolbar.tooltips = false
    }

    add(toolbar)
  end
end
