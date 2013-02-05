/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE rb_cGIUnionInfo
#define SELF(self) (RVAL2GI_UNION_INFO(self))

GType
gi_union_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIUnionInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_n_fields(VALUE self)
{
    GIUnionInfo *info;

    info = SELF(self);
    return INT2NUM(g_union_info_get_n_fields(info));
}

static VALUE
rg_get_field(VALUE self, VALUE rb_n)
{
    GIUnionInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_union_info_get_field(info, n));
}

static VALUE
rg_get_field_value(VALUE self, VALUE rb_union, VALUE rb_n)
{
    GIUnionInfo *info;
    gint n;
    GIFieldInfo *field_info;
    GType gtype;
    VALUE rb_value;

    info = SELF(self);
    n = NUM2INT(rb_n);
    field_info = g_union_info_get_field(info, n);
    gtype = g_registered_type_info_get_g_type(info);
    rb_value = rb_gi_field_info_get_field_raw(field_info,
                                              RVAL2BOXED(rb_union, gtype));
    g_base_info_unref(field_info);

    return rb_value;
}

static VALUE
rg_set_field_value(VALUE self, VALUE rb_union, VALUE rb_n, VALUE rb_value)
{
    GIUnionInfo *info;
    gint n;
    GIFieldInfo *field_info;
    GType gtype;

    info = SELF(self);
    n = NUM2INT(rb_n);
    field_info = g_union_info_get_field(info, n);
    gtype = g_registered_type_info_get_g_type(info);
    rb_gi_field_info_set_field_raw(field_info,
                                   RVAL2BOXED(rb_union, gtype),
                                   rb_value);
    /* TODO: use rb_ensure() to unref field_info. */
    g_base_info_unref(field_info);

    return Qnil;
}

static VALUE
rg_n_methods(VALUE self)
{
    GIUnionInfo *info;

    info = SELF(self);
    return INT2NUM(g_union_info_get_n_methods(info));
}

static VALUE
rg_get_method(VALUE self, VALUE rb_n_or_name)
{
    GIUnionInfo *info;
    GIFunctionInfo *function_info;

    info = SELF(self);
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        gint n;
        n = NUM2INT(rb_n_or_name);
        function_info = g_union_info_get_method(info, n);
    } else {
        const gchar *name;
        name = RVAL2CSTR(rb_n_or_name);
        function_info = g_union_info_find_method(info, name);
    }

    return GI_BASE_INFO2RVAL_WITH_UNREF(function_info);
}

static VALUE
rg_discriminated_p(VALUE self)
{
    GIUnionInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_union_info_is_discriminated(info));
}

static VALUE
rg_discriminator_offset(VALUE self)
{
    GIUnionInfo *info;

    info = SELF(self);
    return INT2NUM(g_union_info_get_discriminator_offset(info));
}

static VALUE
rg_discriminator_type(VALUE self)
{
    GIUnionInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_union_info_get_discriminator_type(info));
}

static VALUE
rg_get_discriminator(VALUE self, VALUE rb_n)
{
    GIUnionInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_union_info_get_discriminator(info, n));
}

static VALUE
rg_size(VALUE self)
{
    GIUnionInfo *info;

    info = SELF(self);
    return ULONG2NUM(g_union_info_get_size(info));
}


static VALUE
rg_alignment(VALUE self)
{
    GIUnionInfo *info;

    info = SELF(self);
    return ULONG2NUM(g_union_info_get_alignment(info));
}

void
rb_gi_union_info_init(VALUE rb_mGI, VALUE rb_cGIRegisteredTypeInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_UNION_INFO, "UnionInfo", rb_mGI,
				rb_cGIRegisteredTypeInfo);

    RG_DEF_METHOD(n_fields, 0);
    RG_DEF_METHOD(get_field, 1);
    RG_DEF_METHOD(get_field_value, 2);
    RG_DEF_METHOD(set_field_value, 3);
    RG_DEF_METHOD(n_methods, 0);
    RG_DEF_METHOD(get_method, 1);
    RG_DEF_METHOD_P(discriminated, 0);
    RG_DEF_METHOD(discriminator_offset, 0);
    RG_DEF_METHOD(discriminator_type, 0);
    RG_DEF_METHOD(get_discriminator, 1);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(alignment, 0);
}
