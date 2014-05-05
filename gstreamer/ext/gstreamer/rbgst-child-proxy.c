/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2014  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cChildProxy

#define SELF(object) (GST_CHILD_PROXY(RVAL2GOBJ(object)))

static void
rg_raise_no_property_error(VALUE object, const gchar *name)
{
    VALUE eNoSuchProperty;

    eNoSuchProperty = rb_const_get(mGLib, rb_intern("NoSuchProperty"));
    rb_raise(eNoSuchProperty,
             "%s: No such property: <%s>",
             rbg_inspect(object),
             name);
}

static VALUE
rg_get_property(VALUE self, VALUE rb_name)
{
    GstChildProxy *proxy;
    const gchar *name;
    GValue value = G_VALUE_INIT;
    VALUE rb_value;
    GObjectClass *g_object_class;
    GParamSpec *property_spec;

    proxy = SELF(self);
    name = RVAL2CSTR_ACCEPT_SYMBOL(rb_name);

    g_object_class = G_OBJECT_GET_CLASS(proxy);
    property_spec = g_object_class_find_property(g_object_class, name);
    if (!property_spec) {
        rg_raise_no_property_error(self, name);
    }

    g_value_init(&value, G_PARAM_SPEC_VALUE_TYPE(property_spec));
    gst_child_proxy_get_property(proxy, name, &value);
    rb_value = GVAL2RVAL(&value);
    g_value_unset(&value);

    return rb_value;
}

static VALUE
rg_set_property(VALUE self, VALUE rb_name, VALUE rb_value)
{
    GstChildProxy *proxy;
    const gchar *name;
    GValue value = G_VALUE_INIT;
    GObjectClass *g_object_class;
    GParamSpec *property_spec;

    proxy = SELF(self);
    name = RVAL2CSTR_ACCEPT_SYMBOL(rb_name);

    g_object_class = G_OBJECT_GET_CLASS(proxy);
    property_spec = g_object_class_find_property(g_object_class, name);
    if (!property_spec) {
        rg_raise_no_property_error(self, name);
    }

    g_value_init(&value, G_PARAM_SPEC_VALUE_TYPE(property_spec));
    rbgobj_rvalue_to_gvalue(rb_value, &value);
    gst_child_proxy_set_property(proxy, name, &value);
    g_value_unset(&value);

    return self;
}

void
rb_gst_init_child_proxy(void)
{
    VALUE mGst;
    VALUE RG_TARGET_NAMESPACE;

    mGst = rb_const_get(rb_cObject, rb_intern("Gst"));
    RG_TARGET_NAMESPACE = rb_const_get(mGst, rb_intern("ChildProxy"));

    rb_remove_method(RG_TARGET_NAMESPACE, "get_property");
    RG_DEF_METHOD(get_property, 1);
    rb_remove_method(RG_TARGET_NAMESPACE, "set_property");
    RG_DEF_METHOD(set_property, 2);
}
