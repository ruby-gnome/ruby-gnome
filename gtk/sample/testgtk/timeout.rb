=begin header

  timeout.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/02/01 16:46:23 $
  $Id: timeout.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

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
# # Timeout Test
# #
# static int timer = 0

# gint
# timeout_test (GtkWidget *label)
#   static int count = 0
#   static char buffer[32]

#   sprintf (buffer, "count: %d", ++count)
#   label.set(buffer)

#   return true
# end

# void
# start_timeout_test (GtkWidget *widget,
# 		    GtkWidget *label)
#   if (!timer)
#     {
#       timer = gtk_timeout_add (100, (GtkFunction) timeout_test, label)
#     }
# end

# void
# stop_timeout_test (GtkWidget *widget,
# 		   gpointer   data)
#   if (timer)
#     {
#       gtk_timeout_remove (timer)
#       timer = 0
#     }
# end

# void
# destroy_timeout_test (GtkWidget  *widget,
# 		      GtkWidget **window)
#   stop_timeout_test (nil, nil)

#  #window = nil
# end

# void
# create_timeout_test ()
#   static GtkWidget *window = nil
#   GtkWidget *button
#   GtkWidget *label

#   if (!window)
#     {
#       window = Gtk::Dialog::new()

#       window.signal_connect("destroy",
# 			  destroy_timeout_test,
# 			  window)

#       window.set_title("Timeout Test")
#       window.border_width(0)

#       label = Gtk::Label::new("count: 0")
#       label.set_padding(10, 10)
#       gtk_box_pack_start (window.vbox, 
# 			  label, true, true, 0)
#       label.show

#       button = Gtk::Button::new("close")
#       gtk_signal_connect_object (button, "clicked",
# 				 gtk_widget_destroy,
# 				 window)
#       button.set_flags(Gtk::Widget::CAN_DEFAULT)
#       gtk_box_pack_start (window.action_area, 
# 			  button, true, true, 0)
#       button.grab_default
#       button.show

#       button = Gtk::Button::new("start")
#       button.signal_connect("clicked",
# 			  start_timeout_test,
# 			  label)
#       button.set_flags(Gtk::Widget::CAN_DEFAULT)
#       gtk_box_pack_start (window.action_area, 
# 			  button, true, true, 0)
#       button.show

#       button = Gtk::Button::new("stop")
#       button.signal_connect("clicked",
# 			  stop_timeout_test,
# 			  nil)
#       button.set_flags(Gtk::Widget::CAN_DEFAULT)
#       gtk_box_pack_start (window.action_area, 
# 			  button, true, true, 0)
#       button.show
#     }

#   if (!window.visible?)
#     window.show
#   else
#     window.destroy
# end
