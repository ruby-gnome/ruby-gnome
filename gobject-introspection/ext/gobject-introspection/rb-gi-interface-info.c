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

#define RG_TARGET_NAMESPACE rb_cGIInterfaceInfo
#define SELF(self) (RVAL2GI_INTERFACE_INFO(self))

GType
gi_interface_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIInterfaceInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_n_prerequisites(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return INT2NUM(g_interface_info_get_n_prerequisites(info));
}

static VALUE
rg_get_prerequisite(VALUE self, VALUE rb_n)
{
    GIInterfaceInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_interface_info_get_prerequisite(info, n));
}

static VALUE
rg_n_properties(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return INT2NUM(g_interface_info_get_n_properties(info));
}

static VALUE
rg_get_property(VALUE self, VALUE rb_n)
{
    GIInterfaceInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_interface_info_get_property(info, n));
}

static VALUE
rg_n_methods(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return INT2NUM(g_interface_info_get_n_methods(info));
}

static VALUE
rg_get_method(VALUE self, VALUE rb_n_or_name)
{
    GIInterfaceInfo *info;
    GIFunctionInfo *function_info;

    info = SELF(self);
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        gint n;
        n = NUM2INT(rb_n_or_name);
        function_info = g_interface_info_get_method(info, n);
    } else {
        const char *name;
        name = RVAL2CSTR(rb_n_or_name);
        function_info = g_interface_info_find_method(info, name);
    }

    return GI_BASE_INFO2RVAL_WITH_UNREF(function_info);
}

static VALUE
rg_n_signals(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return INT2NUM(g_interface_info_get_n_signals(info));
}

static VALUE
rg_get_signal(VALUE self, VALUE rb_n_or_name)
{
    GIInterfaceInfo *info;
    GISignalInfo *signal_info;

    info = SELF(self);
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        gint n;
        n = NUM2INT(rb_n_or_name);
        signal_info = g_interface_info_get_signal(info, n);
    } else {
        const char *name;
        name = RVAL2CSTR(rb_n_or_name);
        signal_info = g_interface_info_find_signal(info, name);
    }

    return GI_BASE_INFO2RVAL_WITH_UNREF(signal_info);
}

static VALUE
rg_n_vfuncs(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return INT2NUM(g_interface_info_get_n_vfuncs(info));
}

static VALUE
rg_get_vfunc(VALUE self, VALUE rb_n_or_name)
{
    GIInterfaceInfo *info;
    GIVFuncInfo *vfunc_info;

    info = SELF(self);
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        gint n;
        n = NUM2INT(rb_n_or_name);
        vfunc_info = g_interface_info_get_vfunc(info, n);
    } else {
        const gchar *name;
        name = RVAL2CSTR(rb_n_or_name);
        vfunc_info = g_interface_info_find_vfunc(info, name);
    }

    return GI_BASE_INFO2RVAL_WITH_UNREF(vfunc_info);
}

static VALUE
rg_n_constants(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return INT2NUM(g_interface_info_get_n_constants(info));
}

static VALUE
rg_get_constant(VALUE self, VALUE rb_n)
{
    GIInterfaceInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_interface_info_get_constant(info, n));
}

static VALUE
rg_iface_struct(VALUE self)
{
    GIInterfaceInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_interface_info_get_iface_struct(info));
}

void
rb_gi_interface_info_init(VALUE rb_mGI, VALUE rb_cGIRegisteredTypeInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_INTERFACE_INFO, "InterfaceInfo", rb_mGI,
				rb_cGIRegisteredTypeInfo);

    RG_DEF_METHOD(n_prerequisites, 0);
    RG_DEF_METHOD(get_prerequisite, 1);
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
    RG_DEF_METHOD(iface_struct, 0);
}
