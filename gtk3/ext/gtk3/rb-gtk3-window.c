/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2017  Ruby-GNOME2 Project Team
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

typedef struct {
    int dummy;
} ToplevelWindowMarkerData;

static void
toplevel_window_marker_mark(G_GNUC_UNUSED void *_)
{
    GList *list = gtk_window_list_toplevels();
    GList *node;
    for (node = list; node; node = g_list_next(node)) {
        rbgobj_gc_mark_instance(node->data);
    }
    g_list_free(list);
}

static void
toplevel_window_marker_free(void *data)
{
    xfree(data);
}

static const rb_data_type_t toplevel_window_marker_type = {
    "Gtk::ToplevelsMarker",
    {
        toplevel_window_marker_mark,
        toplevel_window_marker_free,
        NULL,
        {
            NULL,
            NULL,
        },
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY,
};


void
rbgtk3_window_init(void)
{
    VALUE mGtk;
    VALUE toplevel_window_marker_class;
    VALUE toplevel_window_marker;
    ToplevelWindowMarkerData *data;

    mGtk = rb_const_get(rb_cObject, rb_intern("Gtk"));
    toplevel_window_marker_class =
        rb_define_class_under(mGtk, "ToplevelWindowMarker", rb_cData);
    toplevel_window_marker =
        TypedData_Make_Struct(toplevel_window_marker_class,
                              ToplevelWindowMarkerData,
                              &toplevel_window_marker_type,
                              data);
    rb_gc_register_mark_object(toplevel_window_marker);
}
