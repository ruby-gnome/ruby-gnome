/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2026  Ruby-GNOME Project Team
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

#ifdef HAVE_GIREPOSITORY
#include "rbgi-private.h"

#define RG_TARGET_NAMESPACE rb_cGIInterfaceInfo
#define SELF(self) (RVAL2GI_INTERFACE_INFO(self))

static VALUE
rg_n_prerequisites(VALUE self)
{
    GIInterfaceInfo *info = SELF(self);
    return UINT2NUM(gi_interface_info_get_n_prerequisites(info));
}

static VALUE
rg_get_prerequisite(VALUE self, VALUE rb_n)
{
    GIInterfaceInfo *info = SELF(self);
    unsigned int n = NUM2UINT(rb_n);
    return GOBJ2RVAL_UNREF(gi_interface_info_get_prerequisite(info, n));
}

static VALUE
rg_n_properties(VALUE self)
{
    GIInterfaceInfo *info = SELF(self);
    return UINT2NUM(gi_interface_info_get_n_properties(info));
}

static VALUE
rg_get_property(VALUE self, VALUE rb_n)
{
    GIInterfaceInfo *info = SELF(self);
    unsigned int n = NUM2UINT(rb_n);
    return GOBJ2RVAL_UNREF(gi_interface_info_get_property(info, n));
}

static VALUE
rg_n_methods(VALUE self)
{
    GIInterfaceInfo *info = SELF(self);
    return UINT2NUM(gi_interface_info_get_n_methods(info));
}

static VALUE
rg_get_method(VALUE self, VALUE rb_n_or_name)
{
    GIInterfaceInfo *info = SELF(self);
    GIFunctionInfo *function_info;
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        unsigned int n = NUM2UINT(rb_n_or_name);
        function_info = gi_interface_info_get_method(info, n);
    } else {
        const char *name = RVAL2CSTR(rb_n_or_name);
        function_info = gi_interface_info_find_method(info, name);
    }
    return GOBJ2RVAL_UNREF(function_info);
}

static VALUE
rg_n_signals(VALUE self)
{
    GIInterfaceInfo *info = SELF(self);
    return UINT2NUM(gi_interface_info_get_n_signals(info));
}

static VALUE
rg_get_signal(VALUE self, VALUE rb_n_or_name)
{
    GIInterfaceInfo *info = SELF(self);
    GISignalInfo *signal_info;
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        unsigned int n = NUM2UINT(rb_n_or_name);
        signal_info = gi_interface_info_get_signal(info, n);
    } else {
        const char *name = RVAL2CSTR(rb_n_or_name);
        signal_info = gi_interface_info_find_signal(info, name);
    }
    return GOBJ2RVAL(signal_info);
}

static VALUE
rg_n_vfuncs(VALUE self)
{
    GIInterfaceInfo *info = SELF(self);
    return UINT2NUM(gi_interface_info_get_n_vfuncs(info));
}

static VALUE
rg_get_vfunc(VALUE self, VALUE rb_n_or_name)
{
    GIInterfaceInfo *info = SELF(self);
    GIVFuncInfo *vfunc_info;
    if (RB_TYPE_P(rb_n_or_name, T_FIXNUM)) {
        unsigned int n = NUM2UINT(rb_n_or_name);
        vfunc_info = gi_interface_info_get_vfunc(info, n);
    } else {
        const gchar *name = RVAL2CSTR(rb_n_or_name);
        vfunc_info = gi_interface_info_find_vfunc(info, name);
    }
    return GOBJ2RVAL(vfunc_info);
}

static VALUE
rg_n_constants(VALUE self)
{
    GIInterfaceInfo *info = SELF(self);
    return UINT2NUM(gi_interface_info_get_n_constants(info));
}

static VALUE
rg_get_constant(VALUE self, VALUE rb_n)
{
    GIInterfaceInfo *info = SELF(self);
    unsigned int n = NUM2UINT(rb_n);
    return GOBJ2RVAL_UNREF(gi_interface_info_get_constant(info, n));
}

static VALUE
rg_iface_struct(VALUE self)
{
    GIInterfaceInfo *info = SELF(self);
    return GOBJ2RVAL(gi_interface_info_get_iface_struct(info));
}

void
rbgi_interface_info_init(VALUE rb_mGLib)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS(GI_TYPE_INTERFACE_INFO, "InterfaceInfo", rb_mGLib);

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
#endif
