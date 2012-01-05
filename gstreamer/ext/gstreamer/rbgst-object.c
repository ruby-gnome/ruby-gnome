/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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

#include "rbgst-private.h"

#define RG_TARGET_NAMESPACE cObject
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
rg_set_name(VALUE self, VALUE name)
{
    return CBOOL2RVAL(gst_object_set_name(SELF(self), RVAL2CSTR(name)));
}

void
Init_gst_object(VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE;

    table.type = GST_TYPE_OBJECT;
    table.instance2robj = rbgst_object_instance2robj;
    table.initialize = rbgst_object_initialize;

    RG_DEF_CONVERSION(&table);

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_OBJECT, "Object", mGst);

    RG_DEF_METHOD(set_name, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    G_DEF_CLASS(GST_TYPE_OBJECT_FLAGS, "Flags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_OBJECT_FLAGS, "GST_");
}
