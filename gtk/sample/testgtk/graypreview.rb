=begin header

  graypreview.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/02/01 16:46:23 $
  $Id: graypreview.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

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
# # Gray Preview
# #
# static int gray_idle = 0

# gint
# gray_idle_func (GtkWidget *preview)
#   static int count = 1
#   guchar buf[256]
#   int i, j

#   for (i = 0; i < 256; i++)
#     {
#       for (j = 0; j < 256; j++)
# 	buf[j] = i + j + count

#       preview.draw_row(buf, 0, i, 256)
#     }

#   count += 1

#   preview.draw(nil)

#   return true
# end

# def
# gray_preview_destroy (GtkWidget  *widget,
# 		      GtkWidget **window)
#   gtk_idle_remove (gray_idle)
#   gray_idle = 0

#  #window = nil
# end

# void
# create_gray_preview ()
#   static GtkWidget *window = nil
#   GtkWidget *preview
#   guchar buf[256]
#   int i, j

#   if (!window)
#     {
#       window = Gtk::Window::new(Gtk::WINDOW_TOPLEVEL)

#       window.signal_connect("destroy",
# 			  gray_preview_destroy,
# 			  window)

#       window.set_title("test")
#       window.border_width(10)

#       preview = Gtk::Preview::new(Gtk::PREVIEW_GRAYSCALE)
#       preview.size(256, 256)
#       window.add(preview)
#       preview.show

#       for (i = 0; i < 256; i++)
# 	{
# 	  for (j = 0; j < 256; j++)
# 	    buf[j] = i + j

# 	  preview.draw_row(buf, 0, i, 256)
# 	}

#       gray_idle = gtk_idle_add ((GtkFunction) gray_idle_func, preview)
#     }

#   if (!window.visible?)
#     window.show
#   else
#     window.destroy
# end
