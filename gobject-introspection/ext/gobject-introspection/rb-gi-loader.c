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

#define RG_TARGET_NAMESPACE rb_cGILoader

static const gchar *boxed_class_converters_name = "@@boxed_class_converters";
static const gchar *object_class_converters_name = "@@object_class_converters";

static VALUE
rg_s_define_class(int argc, VALUE *argv, G_GNUC_UNUSED VALUE klass)
{
    VALUE rb_class;
    VALUE rb_gtype, rb_name, rb_module;
    VALUE rb_options, rb_parent, rb_size;
    GType gtype;

    rb_scan_args(argc, argv, "31", &rb_gtype, &rb_name, &rb_module, &rb_options);
    rbg_scan_options(rb_options,
                     "parent", &rb_parent,
                     "size", &rb_size,
                     NULL);

    gtype = rbgobj_gtype_from_ruby(rb_gtype);
    rb_class = G_DEF_CLASS_WITH_PARENT(gtype, RVAL2CSTR(rb_name),
                                       rb_module, rb_parent);
    if (!NIL_P(rb_size)) {
        rb_iv_set(rb_class, "@size", rb_size);
    }
    return rb_class;
}

static VALUE
rg_s_define_interface(G_GNUC_UNUSED VALUE klass,
                      VALUE rb_gtype, VALUE rb_name, VALUE rb_module)
{
    GType gtype;

    gtype = rbgobj_gtype_from_ruby(rb_gtype);
    return G_DEF_INTERFACE(gtype, RVAL2CSTR(rb_name), rb_module);
}

static VALUE
struct_alloc(VALUE klass)
{
    VALUE rb_size;
    gpointer instance = NULL;
    gboolean is_owned;

    rb_size = rb_iv_get(klass, "@size");
    if (NIL_P(rb_size)) {
        is_owned = FALSE;
    } else {
        size_t size;
        size = NUM2ULL(rb_size);
        instance = xcalloc(1, size);
        is_owned = TRUE;
    }
    return rb_gi_struct_new_raw(klass, instance, is_owned);
}

static VALUE
rg_s_define_struct(int argc, VALUE *argv, G_GNUC_UNUSED VALUE klass)
{
    VALUE rb_size, rb_name, rb_module;
    VALUE rb_options, rb_parent;
    VALUE rb_class;

    rb_scan_args(argc, argv, "31", &rb_size, &rb_name, &rb_module, &rb_options);
    rbg_scan_options(rb_options,
                     "parent", &rb_parent,
                     NULL);

    rb_size = rb_to_int(rb_size);
    if (NIL_P(rb_parent)) {
        rb_parent = rb_cObject;
    }
    rb_class = rb_define_class_under(rb_module, RVAL2CSTR(rb_name), rb_parent);
    rb_iv_set(rb_class, "@size", rb_size);
    rb_define_alloc_func(rb_class, struct_alloc);
    return rb_class;
}

static VALUE
rg_s_define_error(int argc, VALUE *argv, G_GNUC_UNUSED VALUE klass)
{
    VALUE rb_domain, rb_name, rb_module;
    VALUE rb_options, rb_parent, rb_gtype;
    GQuark domain;
    const gchar *name;
    GType gtype = G_TYPE_INVALID;

    rb_scan_args(argc, argv, "31",
                 &rb_domain, &rb_name, &rb_module, &rb_options);
    rbg_scan_options(rb_options,
                     "parent", &rb_parent,
                     "gtype", &rb_gtype,
                     NULL);

    if (RB_TYPE_P(rb_domain, RUBY_T_STRING)) {
        domain = g_quark_from_string(RVAL2CSTR(rb_domain));
        if (domain == 0) {
            rb_raise(rb_eArgError,
                     "invalid domain name: <%s>",
                     rbg_inspect(rb_domain));
        }
    } else {
        domain = NUM2UINT(rb_domain);
    }

    name = RVAL2CSTR(rb_name);

    if (NIL_P(rb_parent)) {
        rb_parent = rb_eStandardError;
    }

    if (!NIL_P(rb_gtype)) {
        gtype = rbgobj_gtype_from_ruby(rb_gtype);
    }

    return G_DEF_ERROR(domain, name, rb_module, rb_parent, gtype);
}

static VALUE
rg_s_implement_virtual_function(G_GNUC_UNUSED VALUE klass,
                                VALUE rb_field_info,
                                VALUE rb_implementor_gtype,
                                VALUE rb_vtable_gtype,
                                VALUE rb_method_name)
{
    GIFieldInfo *field_info;
    GType implementor_gtype;
    GType vtable_gtype;
    const gchar *method_name;
    RBGICallback *callback;

    field_info = RVAL2GI_FIELD_INFO(rb_field_info);
    implementor_gtype = rbgobj_gtype_from_ruby(rb_implementor_gtype);
    vtable_gtype = rbgobj_gtype_from_ruby(rb_vtable_gtype);
    method_name = RVAL2CSTR(rb_method_name);

    {
        GITypeInfo *type_info;
        GICallbackInfo *callback_info;

        type_info = g_field_info_get_type(field_info);
        callback_info = g_type_info_get_interface(type_info);
        callback = rb_gi_callback_new(callback_info, method_name);
        g_base_info_unref(callback_info);
        g_base_info_unref(type_info);
    }

    {
        gpointer implementor_struct;
        gpointer vtable_struct;
        gint offset;
        gpointer *method_address;

        implementor_struct = g_type_class_ref(implementor_gtype);
        if (G_TYPE_IS_INTERFACE(vtable_gtype)) {
            vtable_struct = g_type_interface_peek(implementor_struct,
                                                  vtable_gtype);
        } else {
            vtable_struct = implementor_struct;
        }
        offset = g_field_info_get_offset(field_info);
        method_address = G_STRUCT_MEMBER_P(vtable_struct, offset);
        *method_address = callback->closure;
        g_type_class_unref(implementor_struct);
    }

    return Qnil;
}

typedef struct {
    GType type;
    VALUE rb_converters;
    VALUE rb_converter;
} BoxedInstance2RObjData;

static void
boxed_class_converter_free(gpointer user_data)
{
    BoxedInstance2RObjData *data = user_data;
    rb_ary_delete(data->rb_converters, data->rb_converter);
    g_free(data);
}

static VALUE
boxed_instance2robj(gpointer instance, gpointer user_data)
{
    BoxedInstance2RObjData *data = user_data;
    VALUE default_rb_instance;
    VALUE klass;
    gint flags = 0;
    ID id_call;

    default_rb_instance = rbgobj_make_boxed_default(instance, data->type);
    CONST_ID(id_call, "call");
    klass = rb_funcall(data->rb_converter, id_call, 1, default_rb_instance);
    return rbgobj_make_boxed_raw(instance, data->type, klass, flags);
}

static VALUE
rg_s_register_boxed_class_converter(VALUE klass, VALUE rb_gtype)
{
    RGConvertTable table;
    BoxedInstance2RObjData *data;
    VALUE boxed_class_converters;

    memset(&table, 0, sizeof(RGConvertTable));
    table.type = rbgobj_gtype_from_ruby(rb_gtype);
    table.klass = Qnil;
    table.instance2robj = boxed_instance2robj;

    data = g_new(BoxedInstance2RObjData, 1);
    data->type = table.type;
    data->rb_converter = rb_block_proc();
    boxed_class_converters = rb_cv_get(klass, boxed_class_converters_name);
    rb_ary_push(boxed_class_converters, data->rb_converter);
    table.user_data = data;
    table.notify = boxed_class_converter_free;

    rbgobj_convert_define(&table);

    return Qnil;
}

typedef struct {
    GType type;
    VALUE rb_converters;
    VALUE rb_converter;
} ObjectInstance2RObjData;

static void
object_class_converter_free(gpointer user_data)
{
    ObjectInstance2RObjData *data = user_data;
    rb_ary_delete(data->rb_converters, data->rb_converter);
    g_free(data);
}

static VALUE
object_instance2robj(gpointer instance, gpointer user_data)
{
    ObjectInstance2RObjData *data = user_data;
    VALUE existing_rb_instance;
    VALUE default_rb_instance;
    VALUE klass;
    ID id_call;
    VALUE converted_rb_instance;
    ID id_unref;

    existing_rb_instance = rbgobj_get_ruby_object_from_gobject(instance, FALSE);
    if (!NIL_P(existing_rb_instance))
        return existing_rb_instance;

    default_rb_instance = rbgobj_get_ruby_object_from_gobject(instance, TRUE);
    CONST_ID(id_call, "call");
    klass = rb_funcall(data->rb_converter, id_call, 1, default_rb_instance);
    if (klass == rb_class_of(default_rb_instance))
        return default_rb_instance;

    converted_rb_instance = rbgobj_object_alloc_func(klass);
    g_object_ref(instance);
    CONST_ID(id_unref, "unref");
    rb_funcall(default_rb_instance, id_unref, 0);
    rbgobj_gobject_initialize(converted_rb_instance, instance);
    return converted_rb_instance;
}

static VALUE
rg_s_register_object_class_converter(VALUE klass, VALUE rb_gtype)
{
    RGConvertTable table;
    ObjectInstance2RObjData *data;
    VALUE object_class_converters;

    memset(&table, 0, sizeof(RGConvertTable));
    table.type = rbgobj_gtype_from_ruby(rb_gtype);
    table.klass = Qnil;
    table.instance2robj = object_instance2robj;

    data = g_new(ObjectInstance2RObjData, 1);
    data->type = table.type;
    data->rb_converter = rb_block_proc();
    object_class_converters = rb_cv_get(klass, object_class_converters_name);
    rb_ary_push(object_class_converters, data->rb_converter);
    table.user_data = data;
    table.notify = object_class_converter_free;

    rbgobj_convert_define(&table);

    return Qnil;
}

static VALUE
rg_s_register_constant_rename_map(G_GNUC_UNUSED VALUE klass,
                                  VALUE rb_original,
                                  VALUE rb_renamed)
{
    G_RENAME_CONSTANT(RVAL2CSTR(rb_original), RVAL2CSTR(rb_renamed));
    return Qnil;
}

static VALUE
rg_s_start_callback_dispatch_thread(G_GNUC_UNUSED VALUE klass)
{
    rbgutil_start_callback_dispatch_thread();
    return Qnil;
}

static VALUE
rg_s_reference_gobject(int argc, VALUE *argv, G_GNUC_UNUSED VALUE klass)
{
    VALUE rb_gobject;
    VALUE rb_options, rb_sink;
    gpointer gobject;
    gboolean sink;

    rb_scan_args(argc, argv, "11",
                 &rb_gobject, &rb_options);
    rbg_scan_options(rb_options,
                     "sink", &rb_sink,
                     NULL);

    gobject = RVAL2GOBJ(rb_gobject);

    if (NIL_P(rb_sink)) {
        sink = FALSE;
    } else {
        sink = RVAL2CBOOL(rb_sink);
    }

    if (sink) {
        g_object_ref_sink(gobject);
    } else {
        g_object_ref(gobject);
    }

    return Qnil;
}

static VALUE
rg_s_instantiate_gobject_pointer(G_GNUC_UNUSED VALUE klass,
                                 VALUE rb_gobject_pointer)
{
    GObject *gobject;

    gobject = (gpointer)(NUM2ULL(rb_gobject_pointer));

    return GOBJ2RVAL(gobject);
}

void
rb_gi_loader_init(VALUE rb_mGI)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_define_class_under(rb_mGI, "Loader", rb_cObject);

    rb_cv_set(RG_TARGET_NAMESPACE, boxed_class_converters_name, rb_ary_new());
    rb_cv_set(RG_TARGET_NAMESPACE, object_class_converters_name, rb_ary_new());

    RG_DEF_SMETHOD(define_class, -1);
    RG_DEF_SMETHOD(define_interface, 3);
    RG_DEF_SMETHOD(define_struct, -1);
    RG_DEF_SMETHOD(define_error, -1);
    RG_DEF_SMETHOD(implement_virtual_function, 4);
    RG_DEF_SMETHOD(register_boxed_class_converter, 1);
    RG_DEF_SMETHOD(register_object_class_converter, 1);
    RG_DEF_SMETHOD(register_constant_rename_map, 2);
    RG_DEF_SMETHOD(start_callback_dispatch_thread, 0);
    RG_DEF_SMETHOD(reference_gobject, -1);
    RG_DEF_SMETHOD(instantiate_gobject_pointer, 1);
}
