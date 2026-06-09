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

#define RG_TARGET_NAMESPACE rb_cGIRegisteredTypeInfo
#define SELF(self) (RVAL2GI_REGISTERED_TYPE_INFO(self))

GType
gi_registered_type_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIRegisteredTypeInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_type_name(VALUE self)
{
    GIRegisteredTypeInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_registered_type_info_get_type_name(info));
}

static VALUE
rg_type_init(VALUE self)
{
    GIRegisteredTypeInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_registered_type_info_get_type_init(info));
}

static VALUE
rg_gtype(VALUE self)
{
    GIRegisteredTypeInfo *info;

    info = SELF(self);
    return rbgobj_gtype_new(g_registered_type_info_get_g_type(info));
}

void
rb_gi_registered_type_info_init(VALUE rb_mGI, VALUE rb_cGIBaseInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_REGISTERED_TYPE_INFO,
				"RegisteredTypeInfo",
				rb_mGI,
				rb_cGIBaseInfo);

    RG_DEF_METHOD(type_name, 0);
    RG_DEF_METHOD(type_init, 0);
    RG_DEF_METHOD(gtype, 0);

    rb_gi_struct_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_boxed_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_enum_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_object_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_interface_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_union_info_init(rb_mGI, RG_TARGET_NAMESPACE);
}
