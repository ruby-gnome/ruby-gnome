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

#ifdef HAVE_GIREPOSITORY
#include "rbgi-private.h"

#define RG_TARGET_NAMESPACE rb_cGLibBaseInfo
static VALUE RG_TARGET_NAMESPACE;
#define SELF(self) RVAL2GI_BASE_INFO(self)

static void
rbgi_base_info_free(void *data)
{
    GIBaseInfoStack *info = data;
    gi_base_info_unref(info);
}

static const rb_data_type_t rbgi_base_info_type = {
    "GLib::BaseInfo",
    {
        NULL,
        rbgi_base_info_free,
        NULL,
        NULL,
    },
    &rbg_glib_instantiatable_type,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY |
    RUBY_TYPED_WB_PROTECTED |
    RUBY_TYPED_FROZEN_SHAREABLE,
};

static VALUE
rbgi_base_info_alloc_func(VALUE klass)
{
    return TypedData_Wrap_Struct(klass,
                                 &rbgi_base_info_type,
                                 NULL);
}

VALUE
rbgi_base_info_to_ruby(GIBaseInfo *info)
{
    if (!info) {
        return Qnil;
    }

    gi_base_info_ref(info);
    return TypedData_Wrap_Struct(RG_TARGET_NAMESPACE,
                                 &rbgi_base_info_type,
                                 info);
}

VALUE
rbgi_base_info_to_ruby_take(GIBaseInfo *info)
{
    if (!info) {
        return Qnil;
    }

    return TypedData_Wrap_Struct(RG_TARGET_NAMESPACE,
                                 &rbgi_base_info_type,
                                 info);
}

GIBaseInfo *
rbgi_base_info_from_ruby(VALUE rb_info)
{
    GIBaseInfo *info;
    TypedData_Get_Struct(rb_info,
                         GIBaseInfo,
                         &rbgi_base_info_type,
                         info);
    return info;
}

static VALUE
rg_name(VALUE self)
{
    GIBaseInfo *info;

    info = SELF(self);
    return CSTR2RVAL(gi_base_info_get_name(info));
}

static VALUE
rg_namespace(VALUE self)
{
    GIBaseInfo *info;

    info = SELF(self);
    return CSTR2RVAL(gi_base_info_get_namespace(info));
}

static VALUE
rg_container(VALUE self)
{
    GIBaseInfo *info;

    info = SELF(self);
    return rbgi_base_info_to_ruby(gi_base_info_get_container(info));
}

static VALUE
rg_operator_aref(VALUE self, VALUE name)
{
    GIBaseInfo *info;

    info = SELF(self);
    return CSTR2RVAL(gi_base_info_get_attribute(info, RVAL2CSTR(name)));
}

static VALUE
rg_each(VALUE self)
{
    GIAttributeIter iter = {0, };
    GIBaseInfo *info;
    const gchar *name, *value;

    RETURN_ENUMERATOR(self, 0, NULL);

    info = SELF(self);
    while (gi_base_info_iterate_attributes(info, &iter, &name, &value)) {
	VALUE rb_name, rb_value;
	rb_name  = CSTR2RVAL(name);
	rb_value = CSTR2RVAL(value);
	rb_yield(rb_ary_new3(2, rb_name, rb_value));
    }

    return Qnil;
}

void
rbgi_base_info_init(VALUE rb_mGLib)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(GI_TYPE_BASE_INFO, "BaseInfo", rb_mGLib);
    rb_define_alloc_func(RG_TARGET_NAMESPACE, rbgi_base_info_alloc_func);

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(namespace, 0);
    RG_DEF_METHOD(container, 0);
    RG_DEF_METHOD_OPERATOR("[]", aref, 1);
    RG_DEF_METHOD(each, 0);
}
#endif
