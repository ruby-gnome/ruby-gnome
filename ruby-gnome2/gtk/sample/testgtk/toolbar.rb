=begin header

  toolbar.rb - a part of testgtk.c rewritten in ruby-gtk

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Id: toolbar.rb,v 1.1 2002/05/19 12:29:42 mutoh Exp $

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
    set_policy(false, true, true)
    border_width(0)
    realize

    toolbar = Gtk::Toolbar::new(Gtk::ORIENTATION_HORIZONTAL,
                                 Gtk::TOOLBAR_BOTH)
    toolbar.set_button_relief(Gtk::RELIEF_NONE)

    toolbar.append_item("Horizontal", "Horizontal toolbar layout",
                        "Toolbar/Horizontal",
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_orientation(Gtk::ORIENTATION_HORIZONTAL)
    end
    toolbar.append_item("Vertical", "Vertical toolbar layout",
                        "Toolbar/Vertical",
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_orientation(Gtk::ORIENTATION_VERTICAL)
    end

    toolbar.append_space

    toolbar.append_item("Icons", "Only show toolbar icons",
                        "Toolbar/IconsOnly",
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_style(Gtk::TOOLBAR_ICONS)
    end
    toolbar.append_item("Text", "Only show toolbar text", "Toolbar/TextOnly",
                        new_pixmap("test.xpm", window,
                                    style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_style(Gtk::TOOLBAR_TEXT)
    end
    toolbar.append_item("Both", "Show toolbar icons and text", "Toolbar/Both",
                        new_pixmap("test.xpm", window,
                                    style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_style(Gtk::TOOLBAR_BOTH)
    end

    toolbar.append_space

    entry = Gtk::Entry::new()
    entry.show
    toolbar.append_widget(entry, "This is an unusable GtkEntry ;)",
                          "Hey don't click me!!!")

    toolbar.append_space

    toolbar.append_item("Small", "Use small spaces", "Toolbar/Small",
                        new_pixmap("test.xpm", window,
                                    style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_space_size(5)
    end

    toolbar.append_item("Big", "Use big spaces", "Toolbar/Big",
                        new_pixmap("test.xpm", window,
                                    style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_space_size(10)
    end

    toolbar.append_space

    toolbar.append_item("Enable", "Enable tooltips", nil,
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_tooltips(true)
    end
    toolbar.append_item("Disable", "Disable tooltips", nil,
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_tooltips(false)
    end

    toolbar.append_space

    toolbar.append_item("Borders", "Show Borders", nil,
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_button_relief(Gtk::RELIEF_NORMAL)
    end
    toolbar.append_item("Borderless", "Hide Borders", nil,
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_button_relief(Gtk::RELIEF_NONE)
    end

    toolbar.append_space

    toolbar.append_item("Empty", "Empty spaces", nil,
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_space_style(Gtk::Toolbar::SPACE_EMPTY)
    end
    toolbar.append_item("Lines", "Lines in spaces", nil,
                        new_pixmap("test.xpm", window,
                                   style.bg(Gtk::STATE_NORMAL)), nil) do
      toolbar.set_space_style(Gtk::Toolbar::SPACE_LINE)
    end

    add(toolbar)
    toolbar.show
  end
end
