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

#define RG_TARGET_NAMESPACE rb_cGIPropertyInfo
#define SELF(self) RVAL2GI_BASE_INFO(self)

GType
gi_property_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIPropertyInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_flags(VALUE self)
{
    GIPropertyInfo *info;

    info = SELF(self);
    return INT2NUM(g_property_info_get_flags(info));
}

static VALUE
rg_type(VALUE self)
{
    GIPropertyInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_property_info_get_type(info));
}

static VALUE
rg_ownership_transfer(VALUE self)
{
    GIPropertyInfo *info;

    info = SELF(self);
    return GI_TRANSFER2RVAL(g_property_info_get_ownership_transfer(info));
}

void
rb_gi_property_info_init(VALUE rb_mGI, VALUE rb_cGICallableInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_PROPERTY_INFO, "PropertyInfo", rb_mGI,
				rb_cGICallableInfo);

    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(ownership_transfer, 0);
}
