=begin header

  scrolltest.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/02/01 16:46:23 $
  $Id: scroll.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

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

# static int scroll_test_pos = 0.0
# static GdkGC *scroll_test_gc = nil

# static gint
# scroll_test_expose (GtkWidget *widget, GdkEventExpose *event,
# 		    GtkAdjustment *adj)
#   gint i,j
#   gint imin, imax, jmin, jmax
  
#   imin = (event.area.x) / 10
#   imax = (event.area.x + event.area.width + 9) / 10

#   jmin = (adj.value + event.area.y) / 10
#   jmax = (adj.value + event.area.y + event.area.height + 9) / 10

#   gdk_window_clear_area (widget.window,
# 			 event.area.x, event.area.y,
# 			 event.area.width, event.area.height)

#   for (i=imin; i<imax; i++)
#     for (j=jmin; j<jmax; j++)
#       if ((i+j) % 2)
# 	gdk_draw_rectangle (widget.window, 
# 			    widget.style.black_gc,
# 			    true,
# 			    10*i, 10*j - adj.value, 1+i%10, 1+j%10)

#   return true
# end

# def
# scroll_test_configure (GtkWidget *widget, GdkEventConfigure *event,
# 		       GtkAdjustment *adj)
#   adj.page_increment = 0.9 * widget.allocation.height
#   adj.page_size = widget.allocation.height

#   gtk_signal_emit_by_name (adj, "changed")
# end

# def
# scroll_test_adjustment_changed (GtkAdjustment *adj, GtkWidget *widget)
#   gint source_min = adj.value - scroll_test_pos
#   gint source_max = source_min + widget.allocation.height
#   gint dest_min = 0
#   gint dest_max = widget.allocation.height
#   GdkRectangle rect
#   GdkEvent *event

#   scroll_test_pos = adj.value

#   if (!widget)
#     return

#   if (source_min < 0)
#     {
#       rect.x = 0; 
#       rect.y = 0
#       rect.width = widget.allocation.width
#       rect.height = -source_min
#       if (rect.height > widget.allocation.height)
# 	rect.height = widget.allocation.height

#       source_min = 0
#       dest_min = rect.height
#     }
#   else
#     {
#       rect.x = 0
#       rect.y = 2*widget.allocation.height - source_max
#       if (rect.y < 0)
# 	rect.y = 0
#       rect.width = widget.allocation.width
#       rect.height = widget.allocation.height - rect.y

#       source_max = widget.allocation.height
#       dest_max = rect.y
#     }

#   if (source_min != source_max)
#     {
#       if (scroll_test_gc == nil)
# 	{
# 	  scroll_test_gc = Gdk::GC::new(widget.window)
# 	  scroll_test_gc.set_exposures(true)
# 	}

#       gdk_draw_pixmap (widget.window,
# 		       scroll_test_gc,
# 		       widget.window,
# 		       0, source_min,
# 		       0, dest_min,
# 		       widget.allocation.width,
# 		       source_max - source_min)

#       # Make sure graphics expose events are processed before scrolling
#       # again */
      
#       while ((event = gdk_event_get_graphics_expose (widget.window)) != nil)
# 	{
# 	  widget.event(event)
# 	  if (event.expose.count == 0)
# 	    {
# 	      gdk_event_free (event)
# 	      break
# 	    }
# 	  gdk_event_free (event)
# 	}
#     }


#   if (rect.height != 0)
#     widget.draw(rect)
# end


# void
# create_scroll_test ()
#   static GtkWidget *window = nil
#   GtkWidget *hbox
#   GtkWidget *drawing_area
#   GtkWidget *scrollbar
#   GtkWidget *button
#   GtkAdjustment *adj
  
#   if (!window)
#     {
#       window = Gtk::Dialog::new()

#       window.signal_connect("destroy",
# 			  gtk_widget_destroyed,
# 			  window)

#       window.set_title("Scroll Test")
#       window.border_width(0)

#       hbox = Gtk::HBox::new(false, 0)
#       gtk_box_pack_start (window.vbox, hbox,
# 			  true, true, 0)
#       hbox.show

#       drawing_area = Gtk::DrawingArea::new()
#       drawing_area.size(200, 200)
#       hbox.pack_start(drawing_area, true, true, 0)
#       drawing_area.show

#       drawing_area.set_events(Gdk::EXPOSURE_MASK)

#       adj = Gtk::Adjustment::new(0.0, 0.0, 1000.0, 1.0, 180.0, 200.0)
#       scroll_test_pos = 0.0

#       scrollbar = Gtk::VScrollbar::new(adj)
#       hbox.pack_start(scrollbar, false, false, 0)
#       scrollbar.show

#       drawing_area.signal_connect("expose_event",
# 			  scroll_test_expose, adj)
#       drawing_area.signal_connect("configure_event",
# 			  scroll_test_configure, adj)

      
#       adj.signal_connect("value_changed",
# 			  scroll_test_adjustment_changed,
# 			  drawing_area)
      
#       # .. And create some buttons 

#       button = Gtk::Button::new("Quit")
#       gtk_box_pack_start (window.action_area,
# 			  button, true, true, 0)

#       gtk_signal_connect_object (button, "clicked",
# 				 gtk_widget_destroy,
# 				 window)
#       button.show
#     }

#   if (!window.visible?)
#     window.show
#   else
#     window.destroy
# end
