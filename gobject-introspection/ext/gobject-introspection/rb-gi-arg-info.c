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

#include "rb-gobject-introspection.h"

#define RG_TARGET_NAMESPACE rb_cGIArgInfo
#define SELF(self) (RVAL2GI_ARG_INFO(self))

GType
gi_arg_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIArgInfo",
					    (GBoxedCopyFunc)g_base_info_ref,
					    (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_direction(VALUE self)
{
    GIArgInfo *info;

    info = SELF(self);
    return GI_DIRECTION2RVAL(g_arg_info_get_direction(info));
}

static VALUE
rg_caller_allocates_p(VALUE self)
{
    GIArgInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_arg_info_is_caller_allocates(info));
}

static VALUE
rg_return_value_p(VALUE self)
{
    GIArgInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_arg_info_is_return_value(info));
}

static VALUE
rg_optional_p(VALUE self)
{
    GIArgInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_arg_info_is_optional(info));
}

void
rb_gi_arg_info_init(VALUE rb_mGI, VALUE rb_cGIBaseInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_ARG_INFO, "ArgInfo", rb_mGI,
				rb_cGIBaseInfo);

    RG_DEF_METHOD(direction, 0);
    RG_DEF_METHOD_P(caller_allocates, 0);
    RG_DEF_METHOD_P(return_value, 0);
    RG_DEF_METHOD_P(optional, 0);

    G_DEF_CLASS(G_TYPE_I_DIRECTION, "Direction", rb_mGI);
    G_DEF_CLASS(G_TYPE_I_SCOPE_TYPE, "ScopeType", rb_mGI);
    G_DEF_CLASS(G_TYPE_I_TRANSFER, "Transfer", rb_mGI);
}
