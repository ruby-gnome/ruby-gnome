=begin header

  mainloop.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: mainloop.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/02/01 16:46:23 $
  $Id: mainloop.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

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
# # Test of recursive mainloop
# #

# void
# mainloop_destroyed (GtkWidget *w, GtkWidget **window)
#   window = nil
#   gtk_main_quit ()
# end

# void
# create_mainloop ()
#   static GtkWidget *window = nil
#   GtkWidget *label
#   GtkWidget *button

#   if (!window)
#     {
#       window = Gtk::Dialog::new()

#       window.set_title("Test Main Loop")

#       window.signal_connect("destroy",
# 			  mainloop_destroyed,
# 			  window)

#       label = Gtk::Label::new("In recursive main loop...")
#       label.set_padding(20, 20)

#       gtk_box_pack_start (window.vbox, label,
# 			  true, true, 0)
#       label.show

#       button = Gtk::Button::new("Leave")
#       gtk_box_pack_start (window.action_area, button, 
# 			  false, true, 0)

#       gtk_signal_connect_object (button, "clicked",
# 				 gtk_widget_destroy,
# 				 window)

#       button.set_flags(Gtk::Widget::CAN_DEFAULT)
#       button.grab_default

#       button.show
#     }

#   if (!window.visible?)
#     {
#       window.show

#       g_print ("create_mainloop: start\n")
#       gtk_main ()
#       g_print ("create_mainloop: done\n")
#     }
#   else
#     window.destroy
# end
