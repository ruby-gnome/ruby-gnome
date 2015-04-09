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

#define RG_TARGET_NAMESPACE rb_cGIFieldInfo
#define SELF(self) (RVAL2GI_FIELD_INFO(self))

GType
gi_field_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GIFieldInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_flags(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return GI_FIELD_INFO_FLAGS2RVAL(g_field_info_get_flags(info));
}

static VALUE
rg_size(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return INT2NUM(g_field_info_get_size(info));
}

static VALUE
rg_offset(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return INT2NUM(g_field_info_get_offset(info));
}

static VALUE
rg_type(VALUE self)
{
    GIFieldInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_field_info_get_type(info));
}

VALUE
rb_gi_field_info_get_field_raw(GIFieldInfo *info, gpointer memory)
{
    GIArgument argument;
    GITypeInfo *type_info;
    VALUE rb_field_value;

    if (!g_field_info_get_field(info, memory, &argument)) {
        rb_raise(rb_eArgError, "failed to get field value");
    }

    type_info = g_field_info_get_type(info);
    rb_field_value = GI_ARGUMENT2RVAL(&argument, type_info, NULL, NULL, NULL);
    g_base_info_unref(type_info);

    return rb_field_value;
}

void
rb_gi_field_info_set_field_raw(GIFieldInfo *info, gpointer memory,
                               VALUE rb_field_value)
{
    GIArgument field_value;
    GITypeInfo *type_info;
    GITypeTag type_tag;
    gboolean succeeded;

    type_info = g_field_info_get_type(info);
    type_tag = g_type_info_get_tag(type_info);
    RVAL2GI_VALUE_ARGUMENT(&field_value, type_info, rb_field_value, Qnil);

    succeeded = g_field_info_set_field(info, memory, &field_value);
    if (!succeeded) {
        if (type_tag == GI_TYPE_TAG_UTF8) {
            int offset;
            offset = g_field_info_get_offset(info);
            G_STRUCT_MEMBER(gchar *, memory, offset) = field_value.v_string;
            succeeded = TRUE;
        }
    }
    rb_gi_value_argument_free(&field_value, type_info);
    g_base_info_unref(type_info);

    if (!succeeded) {
        rb_raise(rb_eArgError,
                 "failed to set field value: %s[%s]",
                 g_base_info_get_name(info),
                 g_type_tag_to_string(type_tag));
    }
}

static VALUE
rg_get_field(VALUE self, VALUE rb_memory)
{
    GIFieldInfo *info;
    gpointer memory;

    info = SELF(self);
    memory = GUINT_TO_POINTER(NUM2ULONG(rb_memory));
    return rb_gi_field_info_get_field_raw(info, memory);
}

static VALUE
rg_set_field(VALUE self, VALUE rb_memory, VALUE rb_field_value)
{
    GIFieldInfo *info;
    gpointer memory;

    info = SELF(self);
    memory = GUINT_TO_POINTER(NUM2ULONG(rb_memory));
    rb_gi_field_info_set_field_raw(info, memory, rb_field_value);

    return Qnil;
}

void
rb_gi_field_info_init(VALUE rb_mGI, VALUE rb_cGIBaseInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_FIELD_INFO, "FieldInfo", rb_mGI,
				rb_cGIBaseInfo);

    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(offset, 0);
    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(get_field, 1);
    RG_DEF_METHOD(set_field, 2);

    G_DEF_CLASS(G_TYPE_I_FIELD_INFO_FLAGS, "FieldInfoFlags", rb_mGI);
}
