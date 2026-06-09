/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2021  Ruby-GNOME Project Team
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
      case GI_INFO_TYPE_INVALID:
	g_type = GI_TYPE_BASE_INFO;
	break;
      case GI_INFO_TYPE_FUNCTION:
        {
            GIFunctionInfoFlags flags;

            flags = g_function_info_get_flags((GIFunctionInfo *)info);
            if (flags & GI_FUNCTION_IS_METHOD) {
                g_type = GI_TYPE_METHOD_INFO;
            } else if (flags & GI_FUNCTION_IS_CONSTRUCTOR) {
                g_type = GI_TYPE_CONSTRUCTOR_INFO;
            } else {
                g_type = GI_TYPE_FUNCTION_INFO;
            }
        }
	break;
      case GI_INFO_TYPE_CALLBACK:
	g_type = GI_TYPE_CALLBACK_INFO;
	break;
      case GI_INFO_TYPE_STRUCT:
	g_type = GI_TYPE_STRUCT_INFO;
	break;
      case GI_INFO_TYPE_BOXED:
	g_type = GI_TYPE_BOXED_INFO;
	break;
      case GI_INFO_TYPE_ENUM:
	g_type = GI_TYPE_ENUM_INFO;
	break;
      case GI_INFO_TYPE_FLAGS:
	g_type = GI_TYPE_FLAGS_INFO;
	break;
      case GI_INFO_TYPE_OBJECT:
	g_type = GI_TYPE_OBJECT_INFO;
	break;
      case GI_INFO_TYPE_INTERFACE:
	g_type = GI_TYPE_INTERFACE_INFO;
	break;
      case GI_INFO_TYPE_CONSTANT:
	g_type = GI_TYPE_CONSTANT_INFO;
	break;
      case GI_INFO_TYPE_INVALID_0:
	g_type = GI_TYPE_BASE_INFO;
	break;
      case GI_INFO_TYPE_UNION:
	g_type = GI_TYPE_UNION_INFO;
	break;
      case GI_INFO_TYPE_VALUE:
	g_type = GI_TYPE_VALUE_INFO;
	break;
      case GI_INFO_TYPE_SIGNAL:
	g_type = GI_TYPE_SIGNAL_INFO;
	break;
      case GI_INFO_TYPE_VFUNC:
	g_type = GI_TYPE_VFUNC_INFO;
	break;
      case GI_INFO_TYPE_PROPERTY:
	g_type = GI_TYPE_PROPERTY_INFO;
	break;
      case GI_INFO_TYPE_FIELD:
	g_type = GI_TYPE_FIELD_INFO;
	break;
      case GI_INFO_TYPE_ARG:
	g_type = GI_TYPE_ARG_INFO;
	break;
      case GI_INFO_TYPE_TYPE:
	g_type = GI_TYPE_TYPE_INFO;
	break;
      case GI_INFO_TYPE_UNRESOLVED:
	g_type = GI_TYPE_UNRESOLVED_INFO;
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
rg_type(VALUE self)
{
    GIBaseInfo *info;

    info = SELF(self);
    return GI_INFO_TYPE2RVAL(g_base_info_get_type(info));
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
rg_container(VALUE self)
{
    GIBaseInfo *info;

    info = SELF(self);
    return rb_gi_base_info_to_ruby(g_base_info_get_container(info));
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

    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(namespace, 0);
    RG_DEF_METHOD(container, 0);
    RG_DEF_METHOD_OPERATOR("[]", aref, 1);
    RG_DEF_METHOD(each, 0);

    G_DEF_CLASS(G_TYPE_I_INFO_TYPE, "InfoType", rb_mGI);

    rb_gi_callable_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_registered_type_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_constant_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_value_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_property_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_field_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_arg_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_type_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_unresolved_info_init(rb_mGI, RG_TARGET_NAMESPACE);
}
