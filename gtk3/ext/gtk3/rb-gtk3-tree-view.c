/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015  Ruby-GNOME2 Project Team
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

#include "rb-gtk3-private.h"

static void
rb_gtk3_tree_view_mark(gpointer object)
{
    GtkTreeView *tree_view = object;
    GList *node;
    GList *columns;
    GtkTreeSelection *selection;

    columns = gtk_tree_view_get_columns(tree_view);
    for (node = columns; node; node = g_list_next(node)) {
        GtkTreeViewColumn *column = node->data;
        rbgobj_gc_mark_instance(column);
    }
    g_list_free(columns);

    selection = gtk_tree_view_get_selection(tree_view);
    rbgobj_gc_mark_instance(selection);
}

void
rbgtk3_tree_view_init(void)
{
    rbgobj_register_mark_func(GTK_TYPE_TREE_VIEW, rb_gtk3_tree_view_mark);
}
