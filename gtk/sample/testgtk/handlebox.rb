=begin header

  handlebox.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/02/01 16:46:23 $
  $Id: handlebox.rb,v 1.4 2003/02/01 16:46:23 mutoh Exp $

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

# def
# handle_box_child_signal (GtkHandleBox *hb,
# 			 GtkWidget    *child,
# 			 const gchar  *action)
#   printf ("%s: child <%s> %sed\n",
# 	  gtk_type_name (hb),
# 	  gtk_type_name (child),
# 	  action)
# end
class HandleBoxSample < SampleWindow
  def initialize
    super("Handle Box Test")
    set_policy(true, true, false)
    window.border_width(20)

    vbox = Gtk::VBox::new(false, 0)
    add(vbox)
    vbox.show

    label = Gtk::Label::new("Above")
    vbox.add(label)
    label.show

    separator = Gtk::HSeparator::new()
    vbox.add(separator)
    separator.show
    
    hbox = Gtk::HBox::new(false, 10)
    vbox.add(hbox)
    hbox.show

    separator = Gtk::HSeparator::new()
    vbox.add(separator)
    separator.show

    label = Gtk::Label::new("Below")
    vbox.add(label)
    label.show

#     handle_box = gtk_handle_box_new ()
#     hbox.add(handle_box)
#     handle_box.signal_connect(
# 			"child_attached",
# 			handle_box_child_signal,
# 			"attached")
#     handle_box.signal_connect(
# 			"child_detached",
# 			handle_box_child_signal,
# 			"detached")
#     handle_box.show

#     toolbar = make_toolbar (window)
#     handle_box.add(toolbar)
#     toolbar.show

#     handle_box = gtk_handle_box_new ()
#     hbox.add(handle_box)
#     handle_box.signal_connect(
# 			"child_attached",
# 			handle_box_child_signal,
# 			"attached")
#     handle_box.signal_connect(
# 			"child_detached",
# 			handle_box_child_signal,
# 			"detached")
#     handle_box.show

#     handle_box2 = gtk_handle_box_new ()
#     handle_box.add(handle_box2)
#     handle_box2.signal_connect(
# 			"child_attached",
# 			handle_box_child_signal,
# 			"attached")
#     handle_box2.signal_connect(
# 			"child_detached",
# 			handle_box_child_signal,
# 			"detached")
#     handle_box2.show

#     label = Gtk::Label::new("Fooo!")
#     handle_box2.add(label)
#     label.show
  end
end

# def
# reparent_label (GtkWidget *widget,
# 		GtkWidget *new_parent)
#   GtkWidget *label

#   label = widget.get_user_data

#   label.reparent(new_parent)
# end

# def
# set_parent_signal (GtkWidget *child,
# 		   GtkWidget *old_parent,
# 		   gpointer   func_data)
#   g_print ("set_parent for \"%s\": new parent: \"%s\", old parent: \"%s\", data: %d\n",
# 	   gtk_type_name (child),
# 	   child.parent ? gtk_type_name (child.parent) : "nil",
# 	   old_parent ? gtk_type_name (old_parent) : "nil",
# 	   func_data)
# end
