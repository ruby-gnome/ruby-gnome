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
rb_gtk4_cell_layout_mark(gpointer object)
{
    GtkCellLayout *cell_layout = object;

    GList *renderers = gtk_cell_layout_get_cells(cell_layout);
    GList *node;
    for (node = renderers; node; node = g_list_next(node)) {
        GtkCellRenderer *renderer = node->data;
        rbgobj_gc_mark_instance(renderer);
    }
    g_list_free(renderers);
}

void
rb_gtk4_cell_layout_init(void)
{
    rbgobj_register_mark_func(GTK_TYPE_CELL_LAYOUT, rb_gtk4_cell_layout_mark);
}
