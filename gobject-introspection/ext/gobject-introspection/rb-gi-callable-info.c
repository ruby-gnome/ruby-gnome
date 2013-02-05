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

#define RG_TARGET_NAMESPACE rb_cGICallableInfo
#define SELF(self) ((GICallableInfo *)(RVAL2GI_BASE_INFO(self)))

GType
gi_callable_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GICallableInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_return_type(VALUE self)
{
    GICallableInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL(g_callable_info_get_return_type(info));
}

static VALUE
rg_caller_owns(VALUE self)
{
    GICallableInfo *info;

    info = SELF(self);
    return GI_TRANSFER2RVAL(g_callable_info_get_caller_owns(info));
}

static VALUE
rg_may_return_null_p(VALUE self)
{
    GICallableInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_callable_info_may_return_null(info));
}

static VALUE
rg_n_args(VALUE self)
{
    GICallableInfo *info;

    info = SELF(self);
    return INT2NUM(g_callable_info_get_n_args(info));
}

static VALUE
rg_get_arg(VALUE self, VALUE rb_n)
{
    GICallableInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_callable_info_get_arg(info, n));
}

void
rb_gi_callable_info_init(VALUE rb_mGI, VALUE rb_cGIBaseInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
        G_DEF_CLASS_WITH_PARENT(GI_TYPE_CALLABLE_INFO, "CallableInfo", rb_mGI,
                                rb_cGIBaseInfo);

    RG_DEF_METHOD(return_type, 0);
    RG_DEF_METHOD(caller_owns, 0);
    RG_DEF_METHOD_P(may_return_null, 0);
    RG_DEF_METHOD(n_args, 0);
    RG_DEF_METHOD(get_arg, 1);

    rb_gi_function_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_callback_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_signal_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_vfunc_info_init(rb_mGI, RG_TARGET_NAMESPACE);
}
