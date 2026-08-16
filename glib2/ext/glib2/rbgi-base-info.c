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
    GIBaseInfo *info = data;
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

static gpointer
rbgi_base_info_robj2instance(VALUE rb_info, gpointer user_data)
{
    GIBaseInfo *info;
    TypedData_Get_Struct(rb_info,
                         GIBaseInfo,
                         &rbgi_base_info_type,
                         info);
    return info;
}

static VALUE
rbgi_base_info_instance2robj(gpointer instance, gpointer user_data)
{
    GIBaseInfo *info = instance;
    if (!info) {
        return Qnil;
    }

    VALUE klass = RG_TARGET_NAMESPACE;
    GType type = G_TYPE_FROM_INSTANCE(info);
    if (type == GI_TYPE_ARG_INFO) {
        ID id_ArgInfo;
        RUBY_CONST_ID(id_ArgInfo, "ArgInfo");
        klass = rb_const_get(rbg_mGLib(), id_ArgInfo);
    } else if (type == GI_TYPE_CALLABLE_INFO) {
        ID id_CallableInfo;
        RUBY_CONST_ID(id_CallableInfo, "CallableInfo");
        klass = rb_const_get(rbg_mGLib(), id_CallableInfo);
    } else if (type == GI_TYPE_FUNCTION_INFO) {
        ID id_FunctionInfo;
        RUBY_CONST_ID(id_FunctionInfo, "FunctionInfo");
        klass = rb_const_get(rbg_mGLib(), id_FunctionInfo);
    } else if (type == GI_TYPE_INTERFACE_INFO) {
        ID id_InterfaceInfo;
        RUBY_CONST_ID(id_InterfaceInfo, "InterfaceInfo");
        klass = rb_const_get(rbg_mGLib(), id_InterfaceInfo);
    } else if (type == GI_TYPE_OBJECT_INFO) {
        ID id_ObjectInfo;
        RUBY_CONST_ID(id_ObjectInfo, "ObjectInfo");
        klass = rb_const_get(rbg_mGLib(), id_ObjectInfo);
    } else if (type == GI_TYPE_PROPERTY_INFO) {
        ID id_PropertyInfo;
        RUBY_CONST_ID(id_PropertyInfo, "PropertyInfo");
        klass = rb_const_get(rbg_mGLib(), id_PropertyInfo);
    } else if (type == GI_TYPE_SIGNAL_INFO) {
        ID id_SignalInfo;
        RUBY_CONST_ID(id_SignalInfo, "SignalInfo");
        klass = rb_const_get(rbg_mGLib(), id_SignalInfo);
    } else if (type == GI_TYPE_STRUCT_INFO) {
        ID id_StructInfo;
        RUBY_CONST_ID(id_StructInfo, "StructInfo");
        klass = rb_const_get(rbg_mGLib(), id_StructInfo);
    } else if (type == GI_TYPE_TYPE_INFO) {
        ID id_TypeInfo;
        RUBY_CONST_ID(id_TypeInfo, "TypeInfo");
        klass = rb_const_get(rbg_mGLib(), id_TypeInfo);
    } else if (type == GI_TYPE_VFUNC_INFO) {
        ID id_VFuncInfo;
        RUBY_CONST_ID(id_VFuncInfo, "VFuncInfo");
        klass = rb_const_get(rbg_mGLib(), id_VFuncInfo);
    }

    gi_base_info_ref(info);
    return TypedData_Wrap_Struct(klass, &rbgi_base_info_type, info);
}

static void
rbgi_base_info_unref(gpointer instance, gpointer user_data)
{
    GIBaseInfo *info = instance;
    if (!info) {
        return;
    }
    gi_base_info_unref(info);
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
    GIBaseInfo *info = SELF(self);
    return GOBJ2RVAL(gi_base_info_get_container(info));
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

    RGConvertTable table = {
      .type = GI_TYPE_BASE_INFO,
      .klass = RG_TARGET_NAMESPACE,
      .user_data = NULL,
      .notify = NULL,
      .get_superclass = NULL,
      .type_init_hook = NULL,
      .rvalue2gvalue = NULL,
      .gvalue2rvalue = NULL,
      .initialize = NULL,
      .robj2instance = rbgi_base_info_robj2instance,
      .instance2robj = rbgi_base_info_instance2robj,
      .unref = rbgi_base_info_unref,
    };
    RG_DEF_CONVERSION(&table);

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(namespace, 0);
    RG_DEF_METHOD(container, 0);
    RG_DEF_METHOD_OPERATOR("[]", aref, 1);
    RG_DEF_METHOD(each, 0);
}
#endif
