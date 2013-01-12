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

#include "rb-gobject-introspection.h"

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

static gboolean
gobject_based_p(GIFunctionInfo *info)
{
    GIBaseInfo *container_info;
    GIRegisteredTypeInfo *registered_type_info;

    container_info = g_base_info_get_container((GIBaseInfo *)info);
    if (g_base_info_get_type(container_info) != GI_INFO_TYPE_STRUCT) {
        return TRUE;
    }

    registered_type_info = (GIRegisteredTypeInfo *)container_info;
    if (g_registered_type_info_get_type_init(registered_type_info)) {
        return TRUE;
    }

    return FALSE;
}

static VALUE
rg_invoke(int argc, VALUE *argv, VALUE self)
{
    GIFunctionInfo *info;
    GICallableInfo *callable_info;
    GIArgument receiver;
    GIArgument return_value;
    VALUE rb_out_args;
    VALUE rb_return_value;

    info = SELF(self);

    /* TODO: check argc >= 1 */
    if (gobject_based_p(info)) {
        receiver.v_pointer = RVAL2GOBJ(argv[0]);
    } else {
        receiver.v_pointer = DATA_PTR(argv[0]);
    }
    /* TODO: use rb_protect */
    rb_out_args = rb_gi_function_info_invoke_raw(info, &receiver,
                                                 argc - 1, argv + 1,
                                                 &return_value);

    callable_info = (GICallableInfo *)info;
    rb_return_value = GI_RETURN_ARGUMENT2RVAL(&return_value, callable_info);

    if (NIL_P(rb_out_args)) {
        return rb_return_value;
    } else {
        return rb_ary_new3(2, rb_return_value, rb_out_args);
    }
}

void
rb_gi_method_info_init(VALUE rb_mGI, VALUE rb_cGIFunctionInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_METHOD_INFO, "MethodInfo", rb_mGI,
				rb_cGIFunctionInfo);

    RG_DEF_METHOD(invoke, -1);
}
