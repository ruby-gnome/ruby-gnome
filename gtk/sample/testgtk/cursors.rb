=begin header

  cursors.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/02/01 16:46:23 $
  $Id: cursors.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

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
# # Cursors
# #

# static gint
# cursor_expose_event (GtkWidget *widget,
# 		     GdkEvent  *event,
# 		     gpointer   user_data)
#   GtkDrawingArea *darea
#   GdkDrawable *drawable
#   GdkGC *black_gc
#   GdkGC *gray_gc
#   GdkGC *white_gc
#   guint max_width
#   guint max_height

#   g_return_val_if_fail (widget != nil, true)
#   g_return_val_if_fail (widget, true)

#   darea = widget
#   drawable = widget.window
#   white_gc = widget.style.white_gc
#   gray_gc = widget.style.bg_gc[Gtk::STATE_NORMAL]
#   black_gc = widget.style.black_gc
#   max_width = widget.allocation.width
#   max_height = widget.allocation.height

#   gdk_draw_rectangle (drawable, white_gc,
# 		      true,
# 		      0,
# 		      0,
# 		      max_width,
# 		      max_height / 2)

#   gdk_draw_rectangle (drawable, black_gc,
# 		      true,
# 		      0,
# 		      max_height / 2,
# 		      max_width,
# 		      max_height / 2)

#   gdk_draw_rectangle (drawable, gray_gc,
# 		      true,
# 		      max_width / 3,
# 		      max_height / 3,
# 		      max_width / 3,
# 		      max_height / 3)

#   return true
# end

# def
# set_cursor (GtkWidget *spinner,
# 	    GtkWidget *widget)
#   guint c
#   GdkCursor *cursor

#   c = gtk_spin_button_get_value_as_int (spinner, 0, 152)
#   c &= 0xfe

#   cursor = Gdk::Cursor::new(c)
#   gdk_window_set_cursor (widget.window, cursor)
#   cursor.destroy
# end

# static gint
# cursor_event (GtkWidget          *widget,
# 	      GdkEvent           *event,
# 	      GtkSpinButton	 *spinner)
#   if ((event.type == GDK_BUTTON_PRESS) &&
#       ((event.button.button == 1) ||
#        (event.button.button == 3)))
#     {
#       gtk_spin_button_spin (spinner,
# 			    event.button.button == 1 ? Gtk::ARROW_UP : Gtk::ARROW_DOWN,
# 			    spinner.adjustment.step_increment)
#       return true
#     }

#   return false
# end

# def
# create_cursors ()
#   static GtkWidget *window = nil
#   GtkWidget *frame
#   GtkWidget *hbox
#   GtkWidget *main_vbox
#   GtkWidget *vbox
#   GtkWidget *darea
#   GtkWidget *spinner
#   GtkWidget *button
#   GtkWidget *label
#   GtkWidget *any
#   GtkAdjustment *adj

#   if (!window)
#     {
#       window = Gtk::Window::new(Gtk::WINDOW_TOPLEVEL)
      
#       window.signal_connect("destroy",
# 			  gtk_widget_destroyed,
# 			  window)
      
#       window.set_title("Cursors")
      
#       main_vbox = Gtk::VBox::new(false, 5)
#       main_vbox.border_width(0)
#       window.add(main_vbox)

#       vbox =
# 	Gtk::Widget::new(gtk_vbox_get_type (),
# 			"GtkBox::homogeneous", false,
# 			"GtkBox::spacing", 5,
# 			"GtkContainer::border_width", 10,
# 			"GtkWidget::parent", main_vbox,
# 			"GtkWidget::visible", true,
# 			nil)

#       hbox = Gtk::HBox::new(false, 0)
#       hbox.border_width(5)
#       vbox.pack_start(hbox, false, true, 0)
      
#       label = Gtk::Label::new("Cursor Value:")
#       label.set_alignment(0, 0.5)
#       hbox.pack_start(label, false, true, 0)
      
#       adj = (GtkAdjustment *) Gtk::Adjustment::new(0,
# 						  0, 152,
# 						  2,
# 						  10, 0)
#       spinner = gtk_spin_button_new (adj, 0, 0)
#       hbox.pack_start(spinner, true, true, 0)

#       frame =
# 	Gtk::Widget::new(gtk_frame_get_type (),
# 			"GtkFrame::shadow", Gtk::SHADOW_ETCHED_IN,
# 			"GtkFrame::label_xalign", 0.5,
# 			"GtkFrame::label", "Cursor Area",
# 			"GtkContainer::border_width", 10,
# 			"GtkWidget::parent", vbox,
# 			"GtkWidget::visible", true,
# 			nil)

#       darea = Gtk::DrawingArea::new()
#       darea.set_usize(80, 80)
#       frame.add(darea)
#       darea.signal_connect(
# 			  "expose_event",
# 			  cursor_expose_event,
# 			  nil)
#       darea.set_events(Gdk::EXPOSURE_MASK | Gdk::BUTTON_PRESS_MASK)
#       darea.signal_connect(
# 			  "button_press_event",
# 			  cursor_event,
# 			  spinner)
#       darea.show

#       spinner.signal_connect("changed",
# 			  set_cursor,
# 			  darea)

#       any =
# 	Gtk::Widget::new(gtk_hseparator_get_type (),
# 			"GtkWidget::visible", true,
# 			nil)
#       main_vbox.pack_start(any, false, true, 0)
  
#       hbox = Gtk::HBox::new(false, 0)
#       hbox.border_width(10)
#       main_vbox.pack_start(hbox, false, true, 0)

#       button = Gtk::Button::new("Close")
#       gtk_signal_connect_object (button, "clicked",
# 				 gtk_widget_destroy,
# 				 window)
#       hbox.pack_start(button, true, true, 5)

#       window.show_all

#       set_cursor (spinner, darea)
#     }
#   else
#     window.destroy
# end
