=begin header

  toolbar.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Id: toolbar.rb,v 1.6 2003/02/22 17:54:42 mutoh Exp $

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
    set_border_width(0)
    realize

    toolbar = Gtk::Toolbar::new

    toolbar.append("Horizontal", "Horizontal toolbar layout",
                        "Toolbar/Horizontal",
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_orientation(Gtk::ORIENTATION_HORIZONTAL)
    end
    toolbar.append("Vertical", "Vertical toolbar layout",
                        "Toolbar/Vertical",
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_orientation(Gtk::ORIENTATION_VERTICAL)
    end

    toolbar.append_space

    toolbar.append("Icons", "Only show toolbar icons",
                        "Toolbar/IconsOnly",
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_toolbar_style(Gtk::Toolbar::ICONS)
    end
    toolbar.append("Text", "Only show toolbar text", "Toolbar/TextOnly",
                        new_pixmap("test.xpm", window,
                                    style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_toolbar_style(Gtk::Toolbar::TEXT)
    end
    toolbar.append("Both", "Show toolbar icons and text", "Toolbar/Both",
                        new_pixmap("test.xpm", window,
                                    style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_toolbar_style(Gtk::Toolbar::BOTH)
    end

    toolbar.append_space

    entry = Gtk::Entry::new()
    entry.show
    toolbar.append(entry, "This is an unusable GtkEntry ;)",
                          "Hey don't click me!!!")

    toolbar.append_space

    toolbar.append("Enable", "Enable tooltips", nil,
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_tooltips(true)
    end
    toolbar.append("Disable", "Disable tooltips", nil,
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_tooltips(false)
    end

    add(toolbar)
    toolbar.show
  end
end
