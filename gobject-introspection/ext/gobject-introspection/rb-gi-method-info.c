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

#define RG_TARGET_NAMESPACE rb_cGIMethodInfo
#define SELF(self) RVAL2GI_FUNCTION_INFO(self)

GType
gi_method_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIMethodInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_invoke(VALUE self, VALUE rb_receiver, VALUE rb_arguments)
{
    GIFunctionInfo *info;
    VALUE rb_out_args;
    VALUE rb_return_value;

    info = SELF(self);

    /* TODO: use rb_protect */
    rb_out_args = rb_gi_function_info_invoke_raw(info,
                                                 self,
                                                 rb_receiver,
                                                 rb_arguments,
                                                 NULL,
                                                 &rb_return_value);

    if (NIL_P(rb_out_args)) {
        return rb_return_value;
    } else {
        GICallableInfo *callable_info = (GICallableInfo *)info;
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
rb_gi_method_info_init(VALUE rb_mGI, VALUE rb_cGIFunctionInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_METHOD_INFO, "MethodInfo", rb_mGI,
				rb_cGIFunctionInfo);

    RG_DEF_METHOD(invoke, 2);
}
