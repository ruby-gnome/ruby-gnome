/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2021  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE rb_cGIObjectInfo
#define SELF(self) (RVAL2GI_OBJECT_INFO(self))

GType
gi_object_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIObjectInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_type_name(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_object_info_get_type_name(info));
}

static VALUE
rg_type_init(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_object_info_get_type_init(info));
}

static VALUE
rg_abstract_p(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_object_info_get_abstract(info));
}

static VALUE
rg_fundamental_p(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_object_info_get_fundamental(info));
}

static VALUE
rg_parent(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_object_info_get_parent(info));
}

static VALUE
rg_n_interfaces(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return INT2NUM(g_object_info_get_n_interfaces(info));
}

static VALUE
rg_get_interface(VALUE self, VALUE rb_n)
{
    GIObjectInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_object_info_get_interface(info, n));
}

static VALUE
rg_n_fields(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return INT2NUM(g_object_info_get_n_fields(info));
}

static VALUE
rg_get_field(VALUE self, VALUE rb_n)
{
    GIObjectInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_object_info_get_field(info, n));
}

static VALUE
rg_get_field_value(VALUE self, VALUE rb_object, VALUE rb_n)
{
    GIObjectInfo *info;
    gint n;
    GIFieldInfo *field_info;
    VALUE rb_value;

    info = SELF(self);
    n = NUM2INT(rb_n);
    field_info = g_object_info_get_field(info, n);
    rb_value = rb_gi_field_info_get_field_raw(field_info, RVAL2GOBJ(rb_object));
    g_base_info_unref(field_info);

    return rb_value;
}

static VALUE
rg_set_field_value(VALUE self, VALUE rb_object, VALUE rb_n, VALUE rb_value)
{
    GIObjectInfo *info;
    gint n;
    GIFieldInfo *field_info;

    info = SELF(self);
    n = NUM2INT(rb_n);
    field_info = g_object_info_get_field(info, n);
    rb_gi_field_info_set_field_raw(field_info, RVAL2GOBJ(rb_object), rb_value);
    /* TODO: use rb_ensure() to unref field_info. */
    g_base_info_unref(field_info);

    return Qnil;
}

static VALUE
rg_n_properties(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return INT2NUM(g_object_info_get_n_properties(info));
}

static VALUE
rg_get_property(VALUE self, VALUE rb_n)
{
    GIObjectInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_object_info_get_property(info, n));
}

static VALUE
rg_n_methods(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return INT2NUM(g_object_info_get_n_methods(info));
}

static VALUE
rg_get_method(VALUE self, VALUE rb_n_or_name)
{
    GIObjectInfo *info;
    GIFunctionInfo *function_info;

    info = SELF(self);
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        gint n;
        n = NUM2INT(rb_n_or_name);
        function_info = g_object_info_get_method(info, n);
    } else {
        const gchar *name;
        name = RVAL2CSTR(rb_n_or_name);
        function_info = g_object_info_find_method(info, name);
    }

    return GI_BASE_INFO2RVAL_WITH_UNREF(function_info);
}

static VALUE
rg_n_signals(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return INT2NUM(g_object_info_get_n_signals(info));
}

static VALUE
rg_get_signal(VALUE self, VALUE rb_n)
{
    GIObjectInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_object_info_get_signal(info, n));
}

static VALUE
rg_n_vfuncs(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return INT2NUM(g_object_info_get_n_vfuncs(info));
}

static VALUE
rg_get_vfunc(VALUE self, VALUE rb_n_or_name)
{
    GIObjectInfo *info;
    GIVFuncInfo *vfunc_info;

    info = SELF(self);
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        gint n;
        n = NUM2INT(rb_n_or_name);
        vfunc_info = g_object_info_get_vfunc(info, n);
    } else {
        const gchar *name;
        name = RVAL2CSTR(rb_n_or_name);
        vfunc_info = g_object_info_find_vfunc(info, name);
    }

    return GI_BASE_INFO2RVAL_WITH_UNREF(vfunc_info);
}

static VALUE
rg_n_constants(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return INT2NUM(g_object_info_get_n_constants(info));
}

static VALUE
rg_get_constant(VALUE self, VALUE rb_n)
{
    GIObjectInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_object_info_get_constant(info, n));
}

static VALUE
rg_class_struct(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_object_info_get_class_struct(info));
}

static VALUE
rg_unref_function(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_object_info_get_unref_function(info));
}

static VALUE
rg_ref_function(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_object_info_get_ref_function(info));
}

static VALUE
rg_set_value_function(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_object_info_get_set_value_function(info));
}

static VALUE
rg_get_value_function(VALUE self)
{
    GIObjectInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_object_info_get_get_value_function(info));
}

void
rb_gi_object_info_init(VALUE rb_mGI, VALUE rb_cGIRegisteredTypeInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_OBJECT_INFO, "ObjectInfo", rb_mGI,
				rb_cGIRegisteredTypeInfo);

    RG_DEF_METHOD(type_name, 0);
    RG_DEF_METHOD(type_init, 0);
    RG_DEF_METHOD_P(abstract, 0);
    RG_DEF_METHOD_P(fundamental, 0);
    RG_DEF_METHOD(parent, 0);
    RG_DEF_METHOD(n_interfaces, 0);
    RG_DEF_METHOD(get_interface, 1);
    RG_DEF_METHOD(n_fields, 0);
    RG_DEF_METHOD(get_field, 1);
    RG_DEF_METHOD(get_field_value, 2);
    RG_DEF_METHOD(set_field_value, 3);
    RG_DEF_METHOD(n_properties, 0);
    RG_DEF_METHOD(get_property, 1);
    RG_DEF_METHOD(n_methods, 0);
    RG_DEF_METHOD(get_method, 1);
    RG_DEF_METHOD(n_signals, 0);
    RG_DEF_METHOD(get_signal, 1);
    RG_DEF_METHOD(n_vfuncs, 0);
    RG_DEF_METHOD(get_vfunc, 1);
    RG_DEF_METHOD(n_constants, 0);
    RG_DEF_METHOD(get_constant, 1);
    RG_DEF_METHOD(class_struct, 0);
    RG_DEF_METHOD(unref_function, 0);
    RG_DEF_METHOD(ref_function, 0);
    RG_DEF_METHOD(set_value_function, 0);
    RG_DEF_METHOD(get_value_function, 0);
}
