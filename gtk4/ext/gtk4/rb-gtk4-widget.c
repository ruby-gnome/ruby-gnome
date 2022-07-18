/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015-2022  Ruby-GNOME Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rb-gtk4-private.h"

static void
rb_gtk4_widget_mark(gpointer object)
{
    GtkWidget *child = gtk_widget_get_first_child(object);
    while (child) {
        rbgobj_gc_mark_instance(child);
        child = gtk_widget_get_next_sibling(child);
    }
}

void
rb_gtk4_widget_init(void)
{
    rbgobj_register_mark_func(GTK_TYPE_WIDGET, rb_gtk4_widget_mark);
}
