/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2014  Ruby-GNOME2 Project Team
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

#include "rb-gi-private.h"

#define RG_TARGET_NAMESPACE rb_cGIEnumInfo
#define SELF(self) (RVAL2GI_ENUM_INFO(self))

GType
gi_enum_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIEnumInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_n_values(VALUE self)
{
    GIEnumInfo *info;

    info = SELF(self);
    return INT2NUM(g_enum_info_get_n_values(info));
}

static VALUE
rg_get_value(VALUE self, VALUE rb_n)
{
    GIEnumInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_enum_info_get_value(info, n));
}

static VALUE
rg_values(VALUE self)
{
    GIEnumInfo *info;
    gint i, n;
    VALUE rb_values;

    info = SELF(self);

    rb_values = rb_ary_new();
    n = g_enum_info_get_n_values(info);
    for (i = 0; i < n; i++) {
        GIValueInfo *value_info;
        value_info = g_enum_info_get_value(info, i);
        rb_ary_push(rb_values, GI_BASE_INFO2RVAL_WITH_UNREF(value_info));
    }

    return rb_values;
}

static VALUE
rg_n_methods(VALUE self)
{
    GIEnumInfo *info;

    info = SELF(self);
    return INT2NUM(g_enum_info_get_n_methods(info));
}

static VALUE
rg_get_method(VALUE self, VALUE rb_n)
{
    GIEnumInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_enum_info_get_method(info, n));
}

static VALUE
rg_methods(VALUE self)
{
    GIEnumInfo *info;
    gint i, n;
    VALUE rb_methods;

    info = SELF(self);

    rb_methods = rb_ary_new();
    n = g_enum_info_get_n_methods(info);
    for (i = 0; i < n; i++) {
        GIFunctionInfo *function_info;
        function_info = g_enum_info_get_method(info, i);
        rb_ary_push(rb_methods, GI_BASE_INFO2RVAL_WITH_UNREF(function_info));
    }

    return rb_methods;
}

static VALUE
rg_storage_type(VALUE self)
{
    GIEnumInfo *info;

    info = SELF(self);
    return GI_TYPE_TAG2RVAL(g_enum_info_get_storage_type(info));
}

static VALUE
rg_error_domain(VALUE self)
{
    GIEnumInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_enum_info_get_error_domain(info));
}

void
rb_gi_enum_info_init(VALUE rb_mGI, VALUE rb_cGIRegisteredTypeInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_ENUM_INFO, "EnumInfo", rb_mGI,
				rb_cGIRegisteredTypeInfo);

    RG_DEF_METHOD(n_values, 0);
    RG_DEF_METHOD(get_value, 1);
    RG_DEF_METHOD(values, 0);
    RG_DEF_METHOD(n_methods, 0);
    RG_DEF_METHOD(get_method, 1);
    RG_DEF_METHOD(methods, 0);
    RG_DEF_METHOD(storage_type, 0);
    RG_DEF_METHOD(error_domain, 0);

    rb_gi_flags_info_init(rb_mGI, RG_TARGET_NAMESPACE);
}
