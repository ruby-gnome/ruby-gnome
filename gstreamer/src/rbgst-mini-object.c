/*
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

/* Class: Gst::MiniObject
 * Base class for refcounted lightweight objects.
 */

#define SELF(object) (RVAL2GST_MINI_OBJ(object))

static RGFundamental fundamental;
static VALUE rb_cGstMiniObject;

static void
rb_gst_mini_object_free(void *ptr)
{
    if (ptr) {
        gst_mini_object_unref((GstMiniObject *)ptr);
    }
}

static VALUE
rb_gst_mini_object_get_superclass(void)
{
    return rb_cObject;
}

gpointer
rb_gst_mini_object_robj2instance(VALUE object)
{
    gpointer instance;

    if (!RVAL2CBOOL(rb_obj_is_kind_of(object, rb_cGstMiniObject))) {
        rb_raise(rb_eTypeError, "not a Gst::MiniObject");
    }
    Data_Get_Struct(object, GstMiniObject, instance);
    return instance;
}

static VALUE
rb_gst_mini_object_instance2robj(gpointer instance)
{
    VALUE klass;
    GType type;

    type = G_TYPE_FROM_INSTANCE(instance);
    klass = GTYPE2CLASS(type);
    gst_mini_object_ref(instance);
    return Data_Wrap_Struct(klass, NULL, rb_gst_mini_object_free, instance);
}

void
Init_gst_mini_object(void)
{
    fundamental.type = GST_TYPE_MINI_OBJECT;
    fundamental.get_superclass = rb_gst_mini_object_get_superclass;
    fundamental.type_init_hook = NULL;
    fundamental.rvalue2gvalue = NULL;
    fundamental.gvalue2rvalue = NULL;
    fundamental.initialize = NULL;
    fundamental.robj2instance = rb_gst_mini_object_robj2instance;
    fundamental.instance2robj = rb_gst_mini_object_instance2robj;

    G_DEF_FUNDAMENTAL(&fundamental);

    rb_cGstMiniObject = G_DEF_CLASS(GST_TYPE_MINI_OBJECT, "MiniObject", mGst);
}
