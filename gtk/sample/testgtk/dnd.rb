=begin header

  dnd.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  
  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/02/01 16:46:23 $
  $Id: dnd.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

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
# # Drag -N- Drop
# #

# gint
# dnd_drop_destroy_popup (GtkWidget *widget, GtkWindow **window)
#   if(widget) /* I.e. they clicked the close button */
#     gtk_widget_destroy(*window)
#   else {
#     gtk_grab_remove(*window)
#     window = nil
#   }

#   return false
# end

# void
# dnd_drop (GtkWidget *button, GdkEvent *event)
#   static GtkWidget *window = nil
#   GtkWidget *vbox, *lbl, *btn
#   gchar *msg

#   # DND doesn't obey gtk_grab's, so check if we're already displaying
#   # drop modal dialog first
#   #
#   if (window)
#     return

#   window = Gtk::Window::new(Gtk::WINDOW_DIALOG)
#   window.border_width(10)

#   window.signal_connect("destroy",
# 		      dnd_drop_destroy_popup,
# 		      window)
#   window.signal_connect("delete_event",
# 		      gtk_false,
# 		      window)

#   vbox = Gtk::VBox::new(false, 5)

#   # Display message that we got from drop source 
#   msg = g_malloc(strlen(event.dropdataavailable.data)
# 		 + strlen(event.dropdataavailable.data_type) + 100)
#   sprintf(msg, "Drop data of type %s was:\n\n%s",
# 	  event.dropdataavailable.data_type,
# 	  (char *)event.dropdataavailable.data)
#   lbl = Gtk::Label::new(msg)
#   lbl.set_justify(Gtk::JUSTIFY_FILL)
#   g_free(msg)
#   lbl.show
#   vbox.pack_start_defaults(lbl)

#   # Provide an obvious way out of this heinousness 
#   btn = Gtk::Button::new("Continue with life in\nspite of this oppression")
#   gtk_signal_connect_object (btn, "clicked",
# 			     gtk_widget_destroy,
# 			     window)
#   btn.show
#   vbox.pack_start_defaults(btn)

#   window.add(vbox)

#   vbox.show
#   gtk_grab_add(window)
#   window.show
# end

# void
# dnd_drag_request (GtkWidget *button, GdkEvent *event)
# DND_STRING = "Bill Gates demands royalties for\nyour use of his innovation."
#   button.dnd_data_set(event, DND_STRING, strlen(DND_STRING) + 1)
# end

# void
# create_dnd ()
#   static GtkWidget *window = nil
#   GtkWidget *box1
#   GtkWidget *box2
#   GtkWidget *box3
#   GtkWidget *frame
#   GtkWidget *button
#   GtkWidget *separator

#   # For clarity... 
#   char *possible_drag_types[] = {"text/plain"}
#   char *accepted_drop_types[] = {"text/plain"}

#   static GtkWidget *drag_icon = nil
#   static GtkWidget *drop_icon = nil

#   if (!window)
#     {
#       GdkPoint hotspot = {5,5}
      
#       if (!drag_icon)
# 	{
# 	  drag_icon = shape_create_icon ("Modeller.xpm",
# 					 440, 140, 0,0, Gtk::WINDOW_POPUP)
	  
# 	  drag_icon.signal_connect("destroy",
# 			      gtk_widget_destroyed,
# 			      drag_icon)

# 	  drag_icon.hide
# 	}
      
#       if (!drop_icon)
# 	{
# 	  drop_icon = shape_create_icon ("3DRings.xpm",
# 					 440, 140, 0,0, Gtk::WINDOW_POPUP)
	  
# 	  drop_icon.signal_connect("destroy",
# 			      gtk_widget_destroyed,
# 			      drop_icon)

# 	  drop_icon.hide
# 	}

#       gdk_dnd_set_drag_shape(drag_icon.window,
# 			     hotspot,
# 			     drop_icon.window,
# 			     hotspot)

#       window = Gtk::Window::new(Gtk::WINDOW_TOPLEVEL)

#       window.signal_connect("destroy",
# 			  gtk_widget_destroyed,
# 			  window)

#       window.set_title("Drag -N- Drop")
#       window.border_width(0)

#       box1 = Gtk::VBox::new(false, 0)
#       window.add(box1)
#       box1.show

#       box2 = Gtk::HBox::new(false, 5)
#       box2.border_width(10)
#       box1.pack_start(box2, true, true, 0)
#       box2.show

#       frame = Gtk::Frame::new("Drag")
#       box2.pack_start(frame, true, true, 0)
#       frame.show

#       box3 = Gtk::VBox::new(false, 5)
#       box3.border_width(5)
#       frame.add(box3)
#       box3.show

#       #
#       # FROM Button
#       #
#       button = Gtk::Button::new("Drag me!")
#       box3.pack_start(button, false, true, 0)
#       button.show

#       #
#       # currently, the widget has to be realized to
#       # set dnd on it, this needs to change
#       #
#       button.realize
#       button.signal_connect(
# 			  "drag_request_event",
# 			  dnd_drag_request,
# 			  button)
      
#       button.dnd_drag_set(true, possible_drag_types, 1)


#       frame = Gtk::Frame::new("Drop")
#       box2.pack_start(frame, true, true, 0)
#       frame.show

#       box3 = Gtk::VBox::new(false, 5)
#       box3.border_width(5)
#       frame.add(box3)
#       box3.show


#       #
#       # TO Button
#       #
#       button = Gtk::Button::new("To")
#       box3.pack_start(button, false, true, 0)
#       button.show

#       button.realize
#       button.signal_connect(
# 			  "drop_data_available_event",
# 			  dnd_drop,
# 			  button)

#       button.dnd_drop_set(true, accepted_drop_types, 1, false)


#       separator = Gtk::HSeparator::new()
#       box1.pack_start(separator, false, true, 0)
#       separator.show


#       box2 = Gtk::VBox::new(false, 10)
#       box2.border_width(10)
#       box1.pack_start(box2, false, true, 0)
#       box2.show


#       button = Gtk::Button::new("close")

#       gtk_signal_connect_object (button, "clicked",
# 				 gtk_widget_destroy,
# 				 window)

#       box2.pack_start(button, true, true, 0)
#       button.set_flags(Gtk::Widget::CAN_DEFAULT)
#       button.grab_default
#       button.show
#     }

#   if (!window.visible?)
#     window.show
#   else
#     window.destroy
# end
