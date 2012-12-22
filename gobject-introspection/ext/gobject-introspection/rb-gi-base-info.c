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

#define RG_TARGET_NAMESPACE rb_cGIBaseInfo
#define SELF(self) RVAL2GI_BASE_INFO(self)

VALUE
rb_gi_base_info_to_ruby(GIBaseInfo *info)
{
    GType g_type;
    GIInfoType info_type;

    if (!info) {
	return Qnil;
    }

    info_type = g_base_info_get_type(info);
    switch (info_type) {
      case GI_INFO_TYPE_FUNCTION:
	g_type = GI_TYPE_FUNCTION_INFO;
	break;
      case GI_INFO_TYPE_ARG:
	g_type = GI_TYPE_ARG_INFO;
	break;
      case GI_INFO_TYPE_TYPE:
	g_type = GI_TYPE_TYPE_INFO;
	break;
      default:
	g_type = GI_TYPE_BASE_INFO;
	break;
    }

    return BOXED2RVAL(info, g_type);
}

VALUE
rb_gi_base_info_to_ruby_with_unref(GIBaseInfo *info)
{
    VALUE rb_info;

    rb_info = rb_gi_base_info_to_ruby(info);
    if (!NIL_P(rb_info)) {
	g_base_info_unref(info);
    }

    return rb_info;
}

GIBaseInfo *
rb_gi_base_info_from_ruby(VALUE rb_info)
{
    return RVAL2BOXED(rb_info, GI_TYPE_BASE_INFO);
}

static VALUE
rg_name(VALUE self)
{
    GIBaseInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_base_info_get_name(info));
}

static VALUE
rg_namespace(VALUE self)
{
    GIBaseInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_base_info_get_namespace(info));
}

static VALUE
rg_operator_aref(VALUE self, VALUE name)
{
    GIBaseInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_base_info_get_attribute(info, RVAL2CSTR(name)));
}

static VALUE
rg_each(VALUE self)
{
    GIAttributeIter iter = {0, };
    GIBaseInfo *info;
    gchar *name, *value;

    RETURN_ENUMERATOR(self, 0, NULL);

    info = SELF(self);
    while (g_base_info_iterate_attributes(info, &iter, &name, &value)) {
	VALUE rb_name, rb_value;
	rb_name  = CSTR2RVAL(name);
	rb_value = CSTR2RVAL(value);
	rb_yield(rb_ary_new3(2, rb_name, rb_value));
    }

    return Qnil;
}

void
rb_gi_base_info_init(VALUE rb_mGI)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GI_TYPE_BASE_INFO, "BaseInfo", rb_mGI);

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(namespace, 0);
    RG_DEF_METHOD_OPERATOR("[]", aref, 1);
    RG_DEF_METHOD(each, 0);

    rb_gi_callable_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_arg_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_type_info_init(rb_mGI, RG_TARGET_NAMESPACE);
}
