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

static gboolean
rb_gtk3_tree_model_mark_callback(GtkTreeModel *model,
                                 G_GNUC_UNUSED GtkTreePath *path,
                                 GtkTreeIter *iter,
                                 gpointer data)
{
    GArray *target_columns = data;
    guint i, n;

    n = target_columns->len;
    for (i = 0; i < n; i++) {
        gint nth_column;
        GValue value = G_VALUE_INIT;

        nth_column = g_array_index(target_columns, gint, i);
        gtk_tree_model_get_value(model, iter, nth_column, &value);
        rbgobj_gc_mark_gvalue(&value);
        g_value_unset(&value);
    }

    return FALSE;
}

static void
rb_gtk3_tree_model_mark(gpointer object)
{
    GtkTreeModel *tree_model;
    GArray *target_columns;
    gint i, n;

    tree_model = GTK_TREE_MODEL(object);

    target_columns = g_array_new(FALSE, FALSE, sizeof(gint));
    n = gtk_tree_model_get_n_columns(tree_model);
    for (i = 0; i < n; i++) {
        GType column_type = gtk_tree_model_get_column_type(tree_model, i);

        if (G_TYPE_FUNDAMENTAL(column_type) != G_TYPE_OBJECT)
            continue;

        g_array_append_val(target_columns, i);
    }

    gtk_tree_model_foreach(tree_model,
                           rb_gtk3_tree_model_mark_callback,
                           target_columns);

    g_array_unref(target_columns);
}

void
rbgtk3_tree_model_init(void)
{
    rbgobj_register_mark_func(GTK_TYPE_TREE_MODEL, rb_gtk3_tree_model_mark);
}
