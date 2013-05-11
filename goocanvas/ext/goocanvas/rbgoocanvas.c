/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2013  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Vincent Isambart <vincent.isambart@gmail.com>
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

#include "rbgoocanvas.h"

static void
rgoo_canvas_mark(gpointer object)
{
    GooCanvas *canvas = object;
    GooCanvasItem *root_item;

    rbgobj_gc_mark_instance(object);
    root_item = goo_canvas_get_root_item(canvas);
    if (root_item) {
        rbgobj_gc_mark_instance(root_item);
    }
}

void
Init_goocanvas(void)
{
    rbgobj_register_mark_func(GOO_TYPE_CANVAS, rgoo_canvas_mark);
}
