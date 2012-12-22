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

#define RG_TARGET_NAMESPACE rb_cGIStructInfo
#define SELF(self) (RVAL2GI_STRUCT_INFO(self))

GType
gi_struct_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIStructInfo",
					    (GBoxedCopyFunc)g_base_info_ref,
					    (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_n_fields(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return INT2NUM(g_struct_info_get_n_fields(info));
}

void
rb_gi_struct_info_init(VALUE rb_mGI, VALUE rb_cGIRegisteredTypeInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_STRUCT_INFO, "StructInfo", rb_mGI,
				rb_cGIRegisteredTypeInfo);

    RG_DEF_METHOD(n_fields, 0);
}
