/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2013  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cStaticPadTemplate

#define SELF(object) ((GstStaticPadTemplate *)DATA_PTR(object))

static VALUE
rb_gst_static_pad_template_alloc(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, NULL, NULL);
}

static VALUE
rg_name(VALUE self)
{
    GstStaticPadTemplate *template = SELF(self);

    if (!template) {
        return Qnil;
    }

    return CSTR2RVAL(template->name_template);
}

static VALUE
rg_direction(VALUE self)
{
    GstStaticPadTemplate *template = SELF(self);

    if (!template) {
        return Qnil;
    }

    return GENUM2RVAL(template->direction, GST_TYPE_PAD_DIRECTION);
}

static VALUE
rg_presence(VALUE self)
{
    GstStaticPadTemplate *template = SELF(self);

    if (!template) {
        return Qnil;
    }

    return GENUM2RVAL(template->direction, GST_TYPE_PAD_PRESENCE);
}

static VALUE
rg_get(VALUE self)
{
    GstStaticPadTemplate *template = SELF(self);

    if (!template) {
        return Qnil;
    }

    return GOBJ2RVAL_UNREF(gst_static_pad_template_get(template));
}

static VALUE
rg_caps(VALUE self)
{
    GstStaticPadTemplate *template = SELF(self);

    if (!template) {
        return Qnil;
    }

    return GOBJ2RVAL_UNREF(gst_static_pad_template_get_caps(template));
}

void
rb_gst_init_static_pad_template(void)
{
    VALUE mGst;
    VALUE RG_TARGET_NAMESPACE;

    mGst = rb_const_get(rb_cObject, rb_intern("Gst"));
    RG_TARGET_NAMESPACE = rb_define_class_under(mGst, "StaticPadTemplate",
                                                rb_cData);

    rb_define_alloc_func(RG_TARGET_NAMESPACE, rb_gst_static_pad_template_alloc);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(direction, 0);
    RG_DEF_METHOD(presence, 0);

    RG_DEF_METHOD(get, 0);
    RG_DEF_METHOD(caps, 0);
}
