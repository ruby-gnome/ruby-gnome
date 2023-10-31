/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2023  Ruby-GNOME2 Project Team
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

#include "rbgst.h"

static void
rb_gst_element_mark(gpointer object)
{
    GstBus *bus = gst_element_get_bus(GST_ELEMENT(object));
    if (bus) {
        rbgobj_gc_mark_instance(bus);
        gst_object_unref(bus);
    }
}

void
rb_gst_init_element(void)
{
    rbgobj_register_mark_func(GST_TYPE_ELEMENT, rb_gst_element_mark);
}
