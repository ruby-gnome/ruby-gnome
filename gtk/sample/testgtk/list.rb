=begin header

  list.rb - a part of testgtk.c rewritten in ruby-gtk

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2002/05/19 12:39:14 $
  $Id: list.rb,v 1.1 2002/05/19 12:39:14 mutoh Exp $

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
# # GtkList
# #
# def
# list_add (GtkWidget *widget,
# 	  GtkWidget *list)
#   static int i = 1
#   gchar buffer[64]
#   GtkWidget *list_item

#   sprintf (buffer, "added item %d", i++)
#   list_item = Gtk::ListItem::new(buffer)
#   list_item.show
#   list.add(list_item)
# end

# def
# list_remove (GtkWidget *widget,
# 	     GtkWidget *list)
#   GList *tmp_list
#   GList *clear_list

#   tmp_list = list.selection
#   clear_list = nil

#   while (tmp_list)
#     {
#       clear_list = g_list_prepend (clear_list, tmp_list.data)
#       tmp_list = tmp_list.next
#     }

#   clear_list = g_list_reverse (clear_list)

#   list.remove_items(clear_list)

#   g_list_free (clear_list)
# end

# def
# list_clear (GtkWidget *widget,
# 	    GtkWidget *list)
#   list.clear_items(3 - 1, 5 - 1)
# end

# def
# create_list ()
#   static GtkWidget *window = nil
#   static char *list_items[] =
#   {
#     "hello",
#     "world",
#     "blah",
#     "foo",
#     "bar",
#     "argh",
#     "spencer",
#     "is a",
#     "wussy",
#     "programmer",
#   }
#   static int nlist_items = sizeof (list_items) / sizeof (list_items[0])

#   GtkWidget *box1
#   GtkWidget *box2
#   GtkWidget *scrolled_win
#   GtkWidget *list
#   GtkWidget *list_item
#   GtkWidget *button
#   GtkWidget *separator
#   int i

#   if (!window)
#     {
#       window = Gtk::Window::new(Gtk::WINDOW_TOPLEVEL)

#       window.signal_connect("destroy",
# 			  gtk_widget_destroyed,
# 			  window)

#       window.set_title("list")
#       window.border_width(0)


#       box1 = Gtk::VBox::new(false, 0)
#       window.add(box1)
#       box1.show


#       box2 = Gtk::VBox::new(false, 10)
#       box2.border_width(10)
#       box1.pack_start(box2, true, true, 0)
#       box2.show


#       scrolled_win = gtk_scrolled_window_new (nil, nil)
#       gtk_scrolled_window_set_policy (scrolled_win,
# 				      Gtk::POLICY_AUTOMATIC, 
# 				      Gtk::POLICY_AUTOMATIC)
#       box2.pack_start(scrolled_win, true, true, 0)
#       scrolled_win.show

#       list = Gtk::List::new()
#       list.set_selection_mode(Gtk::SELECTION_MULTIPLE)
#       list.set_selection_mode(Gtk::SELECTION_BROWSE)
#       scrolled_win.add(list)
#       list.set_focus_vadjustment(
# 					   gtk_scrolled_window_get_vadjustment (scrolled_win))
#       list.show

#       for (i = 0; i < nlist_items; i++)
# 	{
# 	  list_item = Gtk::ListItem::new(list_items[i])
# 	  list.add(list_item)
# 	  list_item.show
# 	}

#       button = Gtk::Button::new("add")
#       button, Gtk::Widget::CAN_FOCUS
#       button.signal_connect("clicked",
# 			  list_add,
# 			  list)
#       box2.pack_start(button, false, true, 0)
#       button.show

#       button = Gtk::Button::new("clear items 3 - 5")
#       button, Gtk::Widget::CAN_FOCUS
#       button.signal_connect("clicked",
# 			  list_clear,
# 			  list)
#       box2.pack_start(button, false, true, 0)
#       button.show

#       button = Gtk::Button::new("remove")
#       button, Gtk::Widget::CAN_FOCUS
#       button.signal_connect("clicked",
# 			  list_remove,
# 			  list)
#       box2.pack_start(button, false, true, 0)
#       button.show


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
