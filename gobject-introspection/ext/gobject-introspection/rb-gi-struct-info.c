/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2013  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE rb_cGIStructInfo
#define SELF(self) (RVAL2GI_STRUCT_INFO(self))

GType
gi_struct_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIStructInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_n_fields(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return INT2NUM(g_struct_info_get_n_fields(info));
}

static VALUE
rg_get_field(VALUE self, VALUE rb_n)
{
    GIStructInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_struct_info_get_field(info, n));
}

static gpointer
extract_raw_struct(VALUE rb_struct,
                   GIStructInfo *struct_info)
{
    GType gtype;
    gpointer raw_struct;

    gtype = g_registered_type_info_get_g_type(struct_info);
    if (gtype == G_TYPE_NONE && rb_respond_to(rb_struct, rb_intern("gtype"))) {
        VALUE rb_gtype;
        rb_gtype = rb_funcall(rb_struct, rb_intern("gtype"), 0);
        gtype = NUM2ULONG(rb_funcall(rb_gtype, rb_intern("to_i"), 0));
    }
    if (gtype == G_TYPE_NONE) {
        raw_struct = DATA_PTR(rb_struct);
    } else {
        raw_struct = RVAL2BOXED(rb_struct, gtype);
    }

    return raw_struct;
}

static VALUE
rg_get_field_value(VALUE self, VALUE rb_struct, VALUE rb_n)
{
    GIStructInfo *info;
    gpointer raw_struct;
    gint n;
    GIFieldInfo *field_info;
    VALUE rb_value;

    info = SELF(self);
    raw_struct = extract_raw_struct(rb_struct, info);
    n = NUM2INT(rb_n);
    field_info = g_struct_info_get_field(info, n);
    rb_value = rb_gi_field_info_get_field_raw(field_info, raw_struct);
    g_base_info_unref(field_info);

    return rb_value;
}

static VALUE
rg_set_field_value(VALUE self, VALUE rb_struct, VALUE rb_n, VALUE rb_value)
{
    GIStructInfo *info;
    gpointer raw_struct;
    gint n;
    GIFieldInfo *field_info;

    info = SELF(self);
    raw_struct = extract_raw_struct(rb_struct, info);
    n = NUM2INT(rb_n);
    field_info = g_struct_info_get_field(info, n);
    rb_gi_field_info_set_field_raw(field_info, raw_struct, rb_value);
    /* TODO: use rb_ensure() to unref field_info. */
    g_base_info_unref(field_info);

    return Qnil;
}

static VALUE
rg_n_methods(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return INT2NUM(g_struct_info_get_n_methods(info));
}

static VALUE
rg_get_method(VALUE self, VALUE rb_n_or_name)
{
    GIStructInfo *info;
    GIFunctionInfo *function_info;

    info = SELF(self);
    if (RB_TYPE_P(rb_n_or_name, RUBY_T_FIXNUM)) {
        gint n;
        n = NUM2INT(rb_n_or_name);
        function_info = g_struct_info_get_method(info, n);
    } else {
        const gchar *name;
        name = RVAL2CSTR(rb_n_or_name);
        function_info = g_struct_info_find_method(info, name);
    }

    return GI_BASE_INFO2RVAL_WITH_UNREF(function_info);
}

static VALUE
rg_size(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return UINT2NUM(g_struct_info_get_size(info));
}

static VALUE
rg_alignment(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return UINT2NUM(g_struct_info_get_alignment(info));
}

static VALUE
rg_gtype_struct_p(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_struct_info_is_gtype_struct(info));
}

static VALUE
rg_foreign_p(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_struct_info_is_foreign(info));
}

void
rb_gi_struct_info_init(VALUE rb_mGI, VALUE rb_cGIRegisteredTypeInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_STRUCT_INFO, "StructInfo", rb_mGI,
				rb_cGIRegisteredTypeInfo);

    RG_DEF_METHOD(n_fields, 0);
    RG_DEF_METHOD(get_field, 1);
    RG_DEF_METHOD(get_field_value, 2);
    RG_DEF_METHOD(set_field_value, 3);
    RG_DEF_METHOD(n_methods, 0);
    RG_DEF_METHOD(get_method, 1);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(alignment, 0);
    RG_DEF_METHOD_P(gtype_struct, 0);
    RG_DEF_METHOD_P(foreign, 0);
}
