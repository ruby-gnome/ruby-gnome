/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2022  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE rb_cGIConstantInfo
#define SELF(self) (RVAL2GI_CONSTANT_INFO(self))

GType
gi_constant_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIConstantInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_type(VALUE self)
{
    GIConstantInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_constant_info_get_type(info));
}

static VALUE
rg_value(VALUE self)
{
    GIConstantInfo *info = SELF(self);
    GIArgument value;
    G_GNUC_UNUSED gint value_size = g_constant_info_get_value(info, &value);

    RBGIArguments args;
    rb_gi_arguments_init(&args, NULL, Qnil, Qnil, NULL);
    RBGIArgMetadata value_metadata;
    rb_gi_arg_metadata_init_type_info(&value_metadata,
                                      g_constant_info_get_type(info));
    VALUE rb_value = rb_gi_arguments_convert_arg(&args,
                                                 &value,
                                                 &value_metadata,
                                                 FALSE);
    rb_gi_arg_metadata_clear(&value_metadata);
    rb_gi_arguments_clear(&args);
    g_constant_info_free_value(info, &value);

    return rb_value;
}

void
rb_gi_constant_info_init(VALUE rb_mGI, VALUE rb_cGIBaseInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_CONSTANT_INFO, "ConstantInfo", rb_mGI,
				rb_cGIBaseInfo);

    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(value, 0);
}
