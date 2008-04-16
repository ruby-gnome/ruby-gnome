/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 * Copyright (C) 2007 Ruby-GNOME2 Project Team
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"
#include "rbgprivate.h"

#define SELF(self) (RVAL2GST_OBJ(self))

static RGConvertTable table = {0};

/* Class: Gst::Object
 * Basis for the GST object hierarchy.
 */

VALUE
rbgst_object_instance2robj(gpointer instance)
{
    if (GST_OBJECT_IS_FLOATING(instance)) {
        gst_object_ref(instance);
        gst_object_sink(instance);
    }

    return rbgobj_get_ruby_object_from_gobject(instance, TRUE);
}

void
rbgst_object_initialize(VALUE obj, gpointer cobj)
{
    if (GST_OBJECT_IS_FLOATING(cobj)) {
        gst_object_ref(cobj);
        gst_object_sink(cobj);
    }

    rbgobj_gobject_initialize(obj, cobj);
}

static VALUE
object_set_name(VALUE self, VALUE name)
{
    return CBOOL2RVAL(gst_object_set_name(SELF(self), RVAL2CSTR(name)));
}

void
Init_gst_object(void)
{
    VALUE cGstObject;

    table.type = GST_TYPE_OBJECT;
    table.instance2robj = rbgst_object_instance2robj;
    table.initialize = rbgst_object_initialize;

    RG_DEF_CONVERSION(&table);

    cGstObject = G_DEF_CLASS(GST_TYPE_OBJECT, "Object", mGst);

    rb_define_method(cGstObject, "set_name", object_set_name, 1);

    G_DEF_SETTERS(cGstObject);

    G_DEF_CLASS(GST_TYPE_OBJECT_FLAGS, "Flags", cGstObject);
    G_DEF_CONSTANTS(cGstObject, GST_TYPE_OBJECT_FLAGS, "GST_");
}
