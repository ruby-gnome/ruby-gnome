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

#define RG_TARGET_NAMESPACE rb_mGObjectIntrospection

typedef struct {
    ID name;
    GICallableInfo *callable_info;

    ffi_cif cif;
    ffi_closure *closure;
} RBGIVFuncCallbackData;

static ID id_send;
static gboolean is_debug_mode = FALSE;

gboolean
rb_gi_is_debug_mode(void)
{
    return is_debug_mode;
}

static void
find_vfunc_info (GIBaseInfo *vfunc_info,
                 GType implementor_gtype,
                 gpointer *implementor_vtable_ret,
                 GIFieldInfo **field_info_ret)
{
    GIBaseInfo *ancestor_info = NULL;
    GIStructInfo *struct_info = NULL;
    GIFieldInfo *field_info = NULL;

    ancestor_info = g_base_info_get_container(vfunc_info);
    // ancestor_gtype = g_registered_type_info_get_g_type (
    //                       (GIRegisteredTypeInfo *) ancestor_info);
    struct_info = g_object_info_get_class_struct((GIObjectInfo*) ancestor_info);
    *implementor_vtable_ret = g_type_class_ref(implementor_gtype);

    field_info = g_struct_info_find_field(
            struct_info, g_base_info_get_name((GIBaseInfo*) vfunc_info));

    if (field_info != NULL) {
        GITypeInfo *type_info;

        type_info = g_field_info_get_type (field_info);
        if (g_type_info_get_tag (type_info) == GI_TYPE_TAG_INTERFACE) {
            *field_info_ret = field_info;
        } else {
            g_base_info_unref (field_info);
        }
        g_base_info_unref (type_info);
    }

    g_base_info_unref (struct_info);
}

static VALUE
garg2rval(GIArgument *argument, GITypeTag type_tag)
{
    switch (type_tag) {
    case GI_TYPE_TAG_INT32:
        return INT2FIX(argument->v_int32);
    default:
        rb_raise(rb_eTypeError, "garg2rval: not implemented");
        break;
    }
}

static void
rval2garg(VALUE value, GITypeTag type_tag, GIArgument *argument)
{
    switch (type_tag) {
    case GI_TYPE_TAG_INT32:
        argument->v_int32 = FIX2INT(value);
        break;
    default:
        rb_raise(rb_eTypeError, "rval2garg: not implemented");
        break;
    }
}

static void
rval2ffiarg(VALUE value, ffi_arg *arg)
{
    switch (TYPE(value)) {
    case T_NIL:
        break;
    case T_FIXNUM:
    case T_DATA:
        *arg = FIX2INT(value);
        break;
    default:
        rb_raise(rb_eTypeError, "rval2ffiarg: not implemented");
        break;
    }
}

static void
ffi_callback(G_GNUC_UNUSED ffi_cif *cif,
             void *ret,
             void **raw_args,
             void *raw_data)
{
    GIArgument **args = NULL;
    RBGIVFuncCallbackData *data = NULL;
    GObject* receiver = NULL;
    VALUE rb_receiver = 0;
    gint n_args = 0, n_in_args = 0, n_out_args = 0;
    size_t i = 0, j = 0;
    VALUE *in_values = NULL;
    VALUE rb_ret;
    GIArgInfo arg_info;
    GITypeInfo type_info;
    GITypeTag type_tag;
    GIDirection direction;
    gboolean ret_type_is_void;

    args = (GIArgument **) raw_args;
    data = (RBGIVFuncCallbackData *) raw_data;

    receiver = G_OBJECT(args[0]->v_pointer);
    rb_receiver = GOBJ2RVAL(receiver);

    n_args = g_callable_info_get_n_args(data->callable_info);
    in_values = (VALUE *) malloc(n_args * sizeof(VALUE));
    in_values[0] = data->name;
    n_in_args++;

    for (i = 1, j = 1; i < n_args; i++) {
        g_callable_info_load_arg(data->callable_info, i, &arg_info);
        g_arg_info_load_type(&arg_info, &type_info);
        type_tag = g_type_info_get_tag(&type_info);
        direction = g_arg_info_get_direction(&arg_info);

        if (type_tag == GI_TYPE_TAG_VOID) continue;

        if (direction == GI_DIRECTION_OUT || direction == GI_DIRECTION_INOUT)
            n_out_args++;

        if (direction == GI_DIRECTION_OUT) continue;

        in_values[j] = garg2rval(args[i], type_tag);
        j++; n_in_args++;
    }

    // result = rb_funcall(rb_receiver, id_send, 1, data->name);
    rb_ret = rb_funcallv(rb_receiver, id_send, n_in_args, in_values);

    g_callable_info_load_return_type(data->callable_info, &type_info);
    ret_type_is_void = g_type_info_get_tag(&type_info) == GI_TYPE_TAG_VOID;

    if (n_out_args == 0 && ret_type_is_void) {
        // do nothing
    } else if (n_out_args == 0) {
        rval2ffiarg(rb_ret, ret);
    } else if (n_out_args == 1 && ret_type_is_void) {
        for (i = 1; i < n_args; i++) {
            g_callable_info_load_arg(data->callable_info, i, &arg_info);
            g_arg_info_load_type(&arg_info, &type_info);
            type_tag = g_type_info_get_tag(&type_info);
            direction = g_arg_info_get_direction(&arg_info);

            if (type_tag == GI_TYPE_TAG_VOID || direction == GI_DIRECTION_IN)
                continue;

            rval2garg(rb_ret, type_tag, *(GIArgument **) args[i]);
            break;
        }
    } else {
        if (TYPE(rb_ret) != T_ARRAY) {
            rb_raise(rb_eTypeError, "return type should be Array");
        }

        for (i = 1, j = 0; i < n_args; i++) {
            g_callable_info_load_arg(data->callable_info, i, &arg_info);
            g_arg_info_load_type(&arg_info, &type_info);
            type_tag = g_type_info_get_tag(&type_info);
            direction = g_arg_info_get_direction(&arg_info);

            if (type_tag == GI_TYPE_TAG_VOID || direction == GI_DIRECTION_IN)
                continue;

            rval2garg(rb_ary_entry(rb_ret, j), type_tag, *(GIArgument **) args[i]);
            j++;
        }
    }

    free(in_values);
}

static VALUE
rb_gi_hook_up_vfunc(G_GNUC_UNUSED VALUE self,
                    VALUE rb_name,
                    VALUE rb_vfunc_info,
                    VALUE rb_gtype)
{
    GIVFuncInfo *vfunc_info = NULL;
    GType gtype = 0;
    gpointer implementor_vtable = NULL;
    GIFieldInfo *field_info = NULL;

    vfunc_info = RVAL2GI_BASE_INFO(rb_vfunc_info);
    gtype = NUM2LONG(rb_gtype);
    g_assert(G_TYPE_IS_CLASSED(gtype));

    find_vfunc_info(vfunc_info, gtype, &implementor_vtable, &field_info);

    if (field_info != NULL) {
        GITypeInfo *type_info = NULL;
        GIBaseInfo *interface_info = NULL;
        RBGIVFuncCallbackData *data = NULL;
        gint offset = 0;
        gpointer *method_ptr = NULL;

        type_info = g_field_info_get_type(field_info);

        interface_info = g_type_info_get_interface(type_info);
        g_assert(g_base_info_get_type(interface_info) == GI_INFO_TYPE_CALLBACK);

        data = ALLOC(RBGIVFuncCallbackData);
        data->name = rb_name;
        data->callable_info = g_base_info_ref(interface_info);
        data->closure = g_callable_info_prepare_closure(
                interface_info, &(data->cif), ffi_callback, data);

        if (data->closure) {
            offset = g_field_info_get_offset(field_info);
            method_ptr = G_STRUCT_MEMBER_P(implementor_vtable, offset);

            *method_ptr = data->closure;
        }

        g_base_info_unref (interface_info);
        g_base_info_unref (type_info);
        g_base_info_unref (field_info);
    }

    return Qnil;
}

void
Init_gobject_introspection(void)
{
    VALUE RG_TARGET_NAMESPACE;

    id_send = rb_intern("__send__");

    {
        const char *rb_gi_debug_env = getenv("RB_GI_DEBUG");
        if (rb_gi_debug_env && strcmp(rb_gi_debug_env, "yes") == 0) {
            is_debug_mode = TRUE;
        }
    }

    RG_TARGET_NAMESPACE = rb_define_module("GObjectIntrospection");

    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GI_MAJOR_VERSION),
                                INT2FIX(GI_MINOR_VERSION),
                                INT2FIX(GI_MICRO_VERSION)));

    rb_gi_argument_init();

    rb_gi_array_type_init(RG_TARGET_NAMESPACE);
    rb_gi_type_tag_init(RG_TARGET_NAMESPACE);
    rb_gi_base_info_init(RG_TARGET_NAMESPACE);
    rb_gi_repository_init(RG_TARGET_NAMESPACE);
    rb_gi_loader_init(RG_TARGET_NAMESPACE);

    rb_gi_callback_init(RG_TARGET_NAMESPACE);

    rb_define_module_function(RG_TARGET_NAMESPACE, "hook_up_vfunc", rb_gi_hook_up_vfunc, 3);
}
