=begin header

  selection.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/02/01 16:46:23 $
  $Id: selection.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

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
# # Selection Test
# #
# void
# selection_test_received (GtkWidget *list, GtkSelectionData *data)
#   GdkAtom *atoms
#   GtkWidget *list_item
#   GList *item_list
#   int i, l

#   if (data.length < 0)
#     {
#       g_print ("Selection retrieval failed\n")
#       return
#     }
#   if (data.type != GDK_SELECTION_TYPE_ATOM)
#     {
#       g_print ("Selection \"TARGETS\" was not returned as atoms!\n")
#       return
#     }

#   # Clear out any current list items 

#   list.clear_items(0, -1)

#   # Add new items to list 

#   atoms = (GdkAtom *)data.data

#   item_list = nil
#   l = data.length / sizeof (GdkAtom)
#   for (i = 0; i < l; i++)
#     {
#       char *name
#       name = atoms[i].name
#       if (name != nil)
# 	{
# 	  list_item = Gtk::ListItem::new(name)
# 	  g_free (name)
# 	}
#       else
# 	list_item = Gtk::ListItem::new("(bad atom)")

#       list_item.show
#       item_list = g_list_append (item_list, list_item)
#     }

#   list.append_items(item_list)

#   return
# end

# void
# selection_test_get_targets (GtkWidget *widget, GtkWidget *list)
#   static GdkAtom targets_atom = Gdk::NONE

#   if (targets_atom == Gdk::NONE)
#     targets_atom = gdk_atom_intern ("TARGETS", false)

#   gtk_selection_convert (list, GDK_SELECTION_PRIMARY, targets_atom,
# 			 Gdk::CURRENT_TIME)
# end

# void
# create_selection_test ()
#   static GtkWidget *window = nil
#   GtkWidget *button
#   GtkWidget *vbox
#   GtkWidget *scrolled_win
#   GtkWidget *list
#   GtkWidget *label

#   if (!window)
#     {
#       window = Gtk::Dialog::new()

#       window.signal_connect("destroy",
# 			  gtk_widget_destroyed,
# 			  window)

#       window.set_title("Selection Test")
#       window.border_width(0)

#       # Create the list 

#       vbox = Gtk::VBox::new(false, 5)
#       vbox.border_width(10)
#       gtk_box_pack_start (window.vbox, vbox,
# 			  true, true, 0)
#       vbox.show

#       label = Gtk::Label::new("Gets available targets for current selection")
#       vbox.pack_start(label, false, false, 0)
#       label.show

#       scrolled_win = gtk_scrolled_window_new (nil, nil)
#       gtk_scrolled_window_set_policy (scrolled_win,
# 				      Gtk::POLICY_AUTOMATIC, 
# 				      Gtk::POLICY_AUTOMATIC)
#       vbox.pack_start(scrolled_win, true, true, 0)
#       scrolled_win.set_usize(100, 200)
#       scrolled_win.show

#       list = Gtk::List::new()
#       scrolled_win.add(list)

#       list.signal_connect("selection_received",
# 			  selection_test_received, nil)
#       list.show

#       # .. And create some buttons 
#       button = Gtk::Button::new("Get Targets")
#       gtk_box_pack_start (window.action_area,
# 			  button, true, true, 0)

#       button.signal_connect("clicked",
# 			  selection_test_get_targets, list)
#       button.show

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
