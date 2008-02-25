/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
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
#include "rbgst-private.h"

/* Class: Gst::MiniObject
 * Base class for refcounted lightweight objects.
 */

#define SELF(object) (RVAL2GST_MINI_OBJ(object))
#define RVAL2GST_FLAGS(flags) \
    (RVAL2GFLAGS(flags, GST_TYPE_MINI_OBJECT_FLAGS))
#define GST_FLAGS2RVAL(flags) \
    (GFLAGS2RVAL(flags, GST_TYPE_MINI_OBJECT_FLAGS))

VALUE rb_cGstMiniObject;

static RGConvertTable table = {0};

void
_rbgst_mini_object_free(void *ptr)
{
    if (ptr) {
        gst_mini_object_unref((GstMiniObject *)ptr);
    }
}

static VALUE
rbgst_mini_object_get_superclass(void)
{
    return rb_cObject;
}

static VALUE
initialize_with_abstract_check(int argc, VALUE *argv, VALUE self)
{
    GType gtype = CLASS2GTYPE(CLASS_OF(self));

    if (G_TYPE_IS_ABSTRACT(gtype))
        rb_raise(rb_eTypeError, "initializing abstract class");

    return rb_call_super(argc, argv);
}

static void
rbgst_mini_object_type_init_hook(VALUE klass)
{
    if (G_TYPE_IS_ABSTRACT(CLASS2GTYPE(klass)))
        rb_define_method(klass, "initialize",
                         initialize_with_abstract_check, -1);
}

static VALUE
gvalue2rvalue(const GValue *value)
{
    GstMiniObject* obj;
    obj = gst_value_get_mini_object(value);
    return obj ? GOBJ2RVAL(obj) : Qnil;
}

static void
rvalue2gvalue(VALUE value, GValue *result)
{
    gst_value_set_mini_object(result, NIL_P(value) ? NULL : RVAL2GOBJ(value));
}

static void
rbgst_mini_object_initialize(VALUE object, gpointer instance)
{
    DATA_PTR(object) = instance;
}

static gpointer
rbgst_mini_object_robj2instance(VALUE object)
{
    gpointer instance;

    if (!RVAL2CBOOL(rb_obj_is_kind_of(object, rb_cGstMiniObject))) {
        rb_raise(rb_eTypeError, "not a Gst::MiniObject");
    }
    Data_Get_Struct(object, GstMiniObject, instance);
    return instance;
}

static void
rbgst_mini_object_define_class_if_need(VALUE klass, GType type)
{
    _rbgst_define_class_if_need(klass, type, NULL);
}

static VALUE
rbgst_mini_object_instance2robj(gpointer instance)
{
    VALUE klass;
    GType type;

    type = G_TYPE_FROM_INSTANCE(instance);
    klass = GTYPE2CLASS(type);
    rbgst_mini_object_define_class_if_need(klass, type);
    gst_mini_object_ref(instance);
    return Data_Wrap_Struct(klass, NULL, _rbgst_mini_object_free, instance);
}

static void
rbgst_mini_object_unref(gpointer instance)
{
    gst_mini_object_unref(instance);
}

static VALUE
s_allocate(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, _rbgst_mini_object_free, NULL);
}

static VALUE
get_flags(VALUE self)
{
    return GST_FLAGS2RVAL(GST_MINI_OBJECT_FLAGS(SELF(self)));
}

static VALUE
set_flags(VALUE self, VALUE flag)
{
    GST_MINI_OBJECT_FLAGS(SELF(self)) = RVAL2GST_FLAGS(flag);
    return Qnil;
}

static VALUE
raise_flag(VALUE self, VALUE flag)
{
    GST_MINI_OBJECT_FLAG_SET(SELF(self), RVAL2GST_FLAGS(flag));
    return Qnil;
}

static VALUE
lower_flag(VALUE self, VALUE flag)
{
    GST_MINI_OBJECT_FLAG_UNSET(SELF(self), RVAL2GST_FLAGS(flag));
    return Qnil;
}

static VALUE
flag_raised_p(VALUE self, VALUE flag)
{
    return CBOOL2RVAL(GST_MINI_OBJECT_FLAG_IS_SET(SELF(self),
                                                  RVAL2GST_FLAGS(flag)));
}

static VALUE
writable_p(VALUE self)
{
    return CBOOL2RVAL(gst_mini_object_is_writable(SELF(self)));
}

static VALUE
writable_bang(VALUE self)
{
    GstMiniObject *original, *writable;

    original = SELF(self);
    writable = gst_mini_object_make_writable(original);
    DATA_PTR(self) = writable;
    if (original == writable)
        gst_mini_object_unref(original);
    return self;
}

void
Init_gst_mini_object(void)
{
    table.type = GST_TYPE_MINI_OBJECT;
    table.get_superclass = rbgst_mini_object_get_superclass;
    table.type_init_hook = rbgst_mini_object_type_init_hook;
    table.rvalue2gvalue = rvalue2gvalue;
    table.gvalue2rvalue = gvalue2rvalue;
    table.initialize = rbgst_mini_object_initialize;
    table.robj2instance = rbgst_mini_object_robj2instance;
    table.instance2robj = rbgst_mini_object_instance2robj;
    table.unref = rbgst_mini_object_unref;

    RG_DEF_CONVERSION(&table);

    rb_cGstMiniObject = G_DEF_CLASS(GST_TYPE_MINI_OBJECT, "MiniObject", mGst);

    G_DEF_CLASS(GST_TYPE_MINI_OBJECT_FLAGS, "Flags", rb_cGstMiniObject);
    G_DEF_CONSTANTS(rb_cGstMiniObject, GST_TYPE_MINI_OBJECT_FLAGS,
                    "GST_MINI_OBJECT_");

    rb_define_alloc_func(rb_cGstMiniObject, s_allocate);

    rb_define_method(rb_cGstMiniObject, "flags", get_flags, 0);
    rb_define_method(rb_cGstMiniObject, "set_flags", set_flags, 1);
    rb_define_method(rb_cGstMiniObject, "raise_flag", raise_flag, 1);
    rb_define_method(rb_cGstMiniObject, "lower_flag", lower_flag, 1);
    rb_define_method(rb_cGstMiniObject, "flag_raised?", flag_raised_p, 1);
    rb_define_method(rb_cGstMiniObject, "writable?", writable_p, 0);
    rb_define_method(rb_cGstMiniObject, "writable!", writable_bang, 0);

    G_DEF_SETTERS(rb_cGstMiniObject);
}
