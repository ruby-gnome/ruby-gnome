=begin header

  colorpreview.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/01/19 14:28:24 $
  $Id: colorpreview.rb,v 1.3 2003/01/19 14:28:24 mutoh Exp $

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
# # Color Preview
# #
# static int color_idle = 0

# gint
# color_idle_func (GtkWidget *preview)
#   static int count = 1
#   guchar buf[768]
#   int i, j, k

#   for (i = 0; i < 256; i++)
#     {
#       for (j = 0, k = 0; j < 256; j++)
# 	{
# 	  buf[k+0] = i + count
# 	  buf[k+1] = 0
# 	  buf[k+2] = j + count
# 	  k += 3
# 	}

#       preview.draw_row(buf, 0, i, 256)
#     }

#   count += 1

#   preview.draw(nil)

#   return true
# end

# def
# color_preview_destroy (GtkWidget  *widget,
# 		       GtkWidget **window)
#   gtk_idle_remove (color_idle)
#   color_idle = 0

#  #window = nil
# end

# void
# create_color_preview ()
#   static GtkWidget *window = nil
#   GtkWidget *preview
#   guchar buf[768]
#   int i, j, k

#   if (!window)
#     {
#       window = Gtk::Window::new(Gtk::WINDOW_TOPLEVEL)

#       window.signal_connect("destroy",
# 			  color_preview_destroy,
# 			  window)

#       window.set_title("test")
#       window.border_width(10)

#       preview = Gtk::Preview::new(Gtk::PREVIEW_COLOR)
#       preview.size(256, 256)
#       window.add(preview)
#       preview.show

#       for (i = 0; i < 256; i++)
# 	{
# 	  for (j = 0, k = 0; j < 256; j++)
# 	    {
# 	      buf[k+0] = i
# 	      buf[k+1] = 0
# 	      buf[k+2] = j
# 	      k += 3
# 	    }

# 	  preview.draw_row(buf, 0, i, 256)
# 	}

#       color_idle = gtk_idle_add ((GtkFunction) color_idle_func, preview)
#     }

#   if (!window.visible?)
#     window.show
#   else
#     window.destroy
# end
