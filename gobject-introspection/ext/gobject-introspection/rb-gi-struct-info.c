/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2019  Ruby-GNOME Project Team
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

typedef struct {
    gpointer instance;
    gboolean is_owned;
} RBGIStructData;

static void
struct_free(void *data)
{
    RBGIStructData *struct_data = data;
    if (struct_data->is_owned) {
        xfree(struct_data->instance);
    }
    xfree(struct_data);
}

static const rb_data_type_t rb_gi_struct_type = {
    "GObjectIntrospection::Struct",
    {
        NULL,
        struct_free,
        NULL,
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY,
};

VALUE
rb_gi_struct_new_raw(VALUE klass, gpointer instance, gboolean is_owned)
{
    RBGIStructData *data;
    data = RB_ALLOC(RBGIStructData);
    data->instance = instance;
    data->is_owned = is_owned;
    return TypedData_Wrap_Struct(klass, &rb_gi_struct_type, data);
}

gpointer
rb_gi_struct_get_raw(VALUE rb_struct, GType gtype)
{
    gpointer raw_struct;

    if (gtype == G_TYPE_NONE) {
        VALUE rb_struct_class;

        rb_struct_class = rb_class_of(rb_struct);
        if (rb_respond_to(rb_struct_class, rb_intern("gtype"))) {
            VALUE rb_gtype;
            rb_gtype = rb_funcall(rb_struct_class, rb_intern("gtype"), 0);
            gtype = rbgobj_gtype_from_ruby(rb_gtype);
        }
    }
    if (gtype == G_TYPE_NONE) {
        RBGIStructData *data;
        TypedData_Get_Struct(rb_struct,
                             RBGIStructData,
                             &rb_gi_struct_type,
                             data);
        raw_struct = data->instance;
    } else if (gtype == G_TYPE_VARIANT) {
        raw_struct = rbg_variant_from_ruby(rb_struct);
    } else {
        raw_struct = RVAL2BOXED(rb_struct, gtype);
    }

    return raw_struct;
}

VALUE
rb_gi_struct_info_to_ruby(GIStructInfo *info,
                          gpointer object,
                          gboolean is_pointer)
{
    GIBaseInfo *base_info = (GIBaseInfo *)info;
    GIRegisteredTypeInfo *registered_type_info = (GIRegisteredTypeInfo *)info;
    GType gtype;
    const char *namespace;
    const char *name;
    VALUE rb_module;
    VALUE rb_class;

    gtype = g_registered_type_info_get_g_type(registered_type_info);
    if (gtype == G_TYPE_VARIANT) {
        GVariant *variant = object;
        return rbg_variant_to_ruby(variant);
    } else if (gtype != G_TYPE_NONE) {
        return BOXED2RVAL(object, gtype);
    }

    namespace = g_base_info_get_namespace(base_info);
    name = g_base_info_get_name(base_info);

    /* Workaround for old cairo-gobject. 1.14.6 on Ubuntu 16.04
     * doesn't provide GType for cairo_font_options_t. */
    if (strcmp(namespace, "cairo") == 0) {
        gchar *gtype_name;
        GType gtype;

        gtype_name = g_strdup_printf("Cairo%s", name);
        gtype = g_type_from_name(gtype_name);
        g_free(gtype_name);
        return BOXED2RVAL(object, gtype);
    }

    rb_module = rb_const_get(rb_cObject, rb_intern(namespace));
    rb_class = rb_const_get(rb_module, rb_intern(name));
    if (rb_respond_to(rb_class, rb_intern("gtype"))) {
        VALUE rb_gtype;
        GType gtype;

        rb_gtype = rb_funcall(rb_class, rb_intern("gtype"), 0);
        gtype = rbgobj_gtype_from_ruby(rb_gtype);
        return BOXED2RVAL(object, gtype);
    }

    if (is_pointer) {
        return rb_gi_struct_new_raw(rb_class, object, FALSE);
    } else {
        size_t object_size;
        gpointer copied_object;

        object_size = g_struct_info_get_size(info);
        copied_object = xmalloc(object_size);
        memcpy(copied_object, object, object_size);

        return rb_gi_struct_new_raw(rb_class, copied_object, TRUE);
    }
}

gpointer
rb_gi_struct_info_from_ruby(GIStructInfo *info, VALUE rb_object)
{
    GIRegisteredTypeInfo *registerd_type_info = (GIRegisteredTypeInfo *)info;
    GType gtype;

    gtype = g_registered_type_info_get_g_type(registerd_type_info);
    return rb_gi_struct_get_raw(rb_object, gtype);
}

static VALUE
rg_n_fields(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return INT2NUM(g_struct_info_get_n_fields(info));
}

static VALUE
rg_get_field(VALUE self, VALUE rb_n)
{
    GIStructInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_struct_info_get_field(info, n));
}

static VALUE
rg_get_field_value(VALUE self, VALUE rb_struct, VALUE rb_n)
{
    GIStructInfo *info;
    gpointer raw_struct;
    gint n;
    GIFieldInfo *field_info;
    VALUE rb_value;

    info = SELF(self);
    raw_struct = rb_gi_struct_info_from_ruby(info, rb_struct);
    n = NUM2INT(rb_n);
    field_info = g_struct_info_get_field(info, n);
    rb_value = rb_gi_field_info_get_field_raw(field_info, raw_struct);
    g_base_info_unref(field_info);

    return rb_value;
}

static VALUE
rg_set_field_value(VALUE self, VALUE rb_struct, VALUE rb_n, VALUE rb_value)
{
    GIStructInfo *info;
    gpointer raw_struct;
    gint n;
    GIFieldInfo *field_info;

    info = SELF(self);
    raw_struct = rb_gi_struct_info_from_ruby(info, rb_struct);
    n = NUM2INT(rb_n);
    field_info = g_struct_info_get_field(info, n);
    rb_gi_field_info_set_field_raw(field_info, raw_struct, rb_value);
    /* TODO: use rb_ensure() to unref field_info. */
    g_base_info_unref(field_info);

    return Qnil;
}

static VALUE
rg_n_methods(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return INT2NUM(g_struct_info_get_n_methods(info));
}

static VALUE
rg_get_method(VALUE self, VALUE rb_n_or_name)
{
    GIStructInfo *info;
    GIFunctionInfo *function_info;

    info = SELF(self);
    if (RB_TYPE_P(rb_n_or_name, RUBY_T_FIXNUM)) {
        gint n;
        n = NUM2INT(rb_n_or_name);
        function_info = g_struct_info_get_method(info, n);
    } else {
        const gchar *name;
        name = RVAL2CSTR(rb_n_or_name);
        function_info = g_struct_info_find_method(info, name);
    }

    return GI_BASE_INFO2RVAL_WITH_UNREF(function_info);
}

static VALUE
rg_size(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return UINT2NUM(g_struct_info_get_size(info));
}

static VALUE
rg_alignment(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return UINT2NUM(g_struct_info_get_alignment(info));
}

static VALUE
rg_gtype_struct_p(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_struct_info_is_gtype_struct(info));
}

static VALUE
rg_foreign_p(VALUE self)
{
    GIStructInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_struct_info_is_foreign(info));
}

void
rb_gi_struct_info_init(VALUE rb_mGI, VALUE rb_cGIRegisteredTypeInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_STRUCT_INFO, "StructInfo", rb_mGI,
				rb_cGIRegisteredTypeInfo);

    RG_DEF_METHOD(n_fields, 0);
    RG_DEF_METHOD(get_field, 1);
    RG_DEF_METHOD(get_field_value, 2);
    RG_DEF_METHOD(set_field_value, 3);
    RG_DEF_METHOD(n_methods, 0);
    RG_DEF_METHOD(get_method, 1);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(alignment, 0);
    RG_DEF_METHOD_P(gtype_struct, 0);
    RG_DEF_METHOD_P(foreign, 0);
}
