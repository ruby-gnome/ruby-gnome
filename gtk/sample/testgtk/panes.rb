=begin header

  panes.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/02/01 16:46:23 $
  $Id: panes.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

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

# #
# # GtkPanes
# #
# void
# create_panes ()
#   static GtkWidget *window = nil
#   GtkWidget *frame
#   GtkWidget *hpaned
#   GtkWidget *vpaned
#   GtkWidget *button

#   if (!window)
#     {
#       window = Gtk::Window::new(Gtk::WINDOW_TOPLEVEL)

#       window.signal_connect("destroy",
# 			  gtk_widget_destroyed,
# 			  window)

#       window.set_title("Panes")
#       window.border_width(0)

#       vpaned = Gtk::VPaned::new()
#       window.add(vpaned)
#       vpaned.border_width(5)
#       vpaned.show

#       hpaned = Gtk::HPaned::new()
#       vpaned.add1(hpaned)

#       frame = Gtk::Frame::new(nil)
#       frame.set_shadow_type(Gtk::SHADOW_IN)
#       frame.set_usize(60, 60)
#       hpaned.add1(frame)
#       frame.show
      
#       button = Gtk::Button::new("Hi there")
#       frame.add(button)
#       button.show

#       frame = Gtk::Frame::new(nil)
#       frame.set_shadow_type(Gtk::SHADOW_IN)
#       frame.set_usize(80, 60)
#       hpaned.add2(frame)
#       frame.show

#       hpaned.show

#       frame = Gtk::Frame::new(nil)
#       frame.set_shadow_type(Gtk::SHADOW_IN)
#       frame.set_usize(60, 80)
#       vpaned.add2(frame)
#       frame.show
#     }

#   if (!window.visible?)
#     window.show
#   else
#     window.destroy
# end
