/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2023  Ruby-GNOME Project Team
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
rb_gtk4_single_selection_mark(gpointer object)
{
    GListModel *model = gtk_single_selection_get_model(object);
    if (model) {
        rbgobj_gc_mark_instance(model);
    }
}

void
rb_gtk4_single_selection_init(void)
{
    rbgobj_register_mark_func(GTK_TYPE_SINGLE_SELECTION,
                              rb_gtk4_single_selection_mark);
}
