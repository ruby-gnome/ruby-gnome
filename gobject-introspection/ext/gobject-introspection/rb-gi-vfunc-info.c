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

#include "rb-gi-private.h"

#define RG_TARGET_NAMESPACE rb_cGIVFuncInfo
#define SELF(self) RVAL2GI_VFUNC_INFO(self)

GType
gi_vfunc_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIVFuncInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_flags(VALUE self)
{
    GIVFuncInfo *info;

    info = SELF(self);
    return GI_VFUNC_INFO_FLAGS2RVAL(g_vfunc_info_get_flags(info));
}

static VALUE
rg_offset(VALUE self)
{
    GIVFuncInfo *info;

    info = SELF(self);
    return INT2NUM(g_vfunc_info_get_offset(info));
}

static VALUE
rg_signal(VALUE self)
{
    GIVFuncInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_vfunc_info_get_signal(info));
}

static VALUE
rg_invoker(VALUE self)
{
    GIVFuncInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_vfunc_info_get_invoker(info));
}

static VALUE
rg_invoke(VALUE self,
          VALUE rb_implementor_gtype,
          VALUE rb_receiver,
          VALUE rb_arguments)
{
    GIVFuncInfo *info = SELF(self);
    GICallableInfo *callable_info = (GICallableInfo *)info;
    GType implementor_gtype = rbgobj_gtype_from_ruby(rb_implementor_gtype);
    VALUE rb_return_value;
    /* TODO: use rb_protect */
    VALUE rb_out_args = rb_gi_callable_info_invoke_raw(info,
                                                       self,
                                                       implementor_gtype,
                                                       rb_receiver,
                                                       rb_arguments,
                                                       NULL,
                                                       &rb_return_value);

    if (NIL_P(rb_out_args)) {
        return rb_return_value;
    } else {
        GITypeInfo return_value_info;
        g_callable_info_load_return_type(callable_info, &return_value_info);
        if (g_type_info_get_tag(&return_value_info) != GI_TYPE_TAG_VOID) {
            rb_ary_unshift(rb_out_args, rb_return_value);
        }
        if (RARRAY_LEN(rb_out_args) == 1) {
            return RARRAY_PTR(rb_out_args)[0];
        } else {
            return rb_out_args;
        }
    }
}

void
rb_gi_vfunc_info_init(VALUE rb_mGI, VALUE rb_cGICallableInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_VFUNC_INFO, "VFuncInfo", rb_mGI,
				rb_cGICallableInfo);

    rb_define_const(RG_TARGET_NAMESPACE, "UNKNOWN_OFFSET", INT2NUM(0xFFFF));

    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(offset, 0);
    RG_DEF_METHOD(signal, 0);
    RG_DEF_METHOD(invoker, 0);
    RG_DEF_METHOD(invoke, 3);

    G_DEF_CLASS(G_TYPE_IV_FUNC_INFO_FLAGS, "VFuncInfoFlags", rb_mGI);
}
