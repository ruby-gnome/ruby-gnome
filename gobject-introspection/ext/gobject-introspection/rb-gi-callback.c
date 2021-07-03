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

struct RBGICallbackData_ {
    RBGICallback *callback;
    RBGIArgMetadata *metadata;
    VALUE rb_callback;
    GObject *owner;
    VALUE rb_owner;
};

typedef struct {
    RBGIArguments *args;
    RBGICallback *callback;
    RBGICallbackData *callback_data;
    void *return_value;
    VALUE rb_return_value;
} RBGICallbackInvokeData;

static GPtrArray *callback_finders;
static VALUE mGLibObject = Qnil;
static VALUE mGI = Qnil;

static VALUE
rb_gi_callback_invoke_without_protect(VALUE user_data)
{
    RBGICallbackInvokeData *data = (RBGICallbackInvokeData *)user_data;
    VALUE rb_args = rb_gi_arguments_in_to_ruby(data->args);

    if (data->callback->method_name) {
        ID id___send__;
        VALUE rb_receiver = rb_ary_shift(rb_args);
        CONST_ID(id___send__, "__send__");
        rb_ary_unshift(rb_args, rb_str_new_cstr(data->callback->method_name));
        data->rb_return_value =
            rb_funcallv(rb_receiver,
                        id___send__,
                        RARRAY_LENINT(rb_args),
                        RARRAY_CONST_PTR(rb_args));
    } else {
        ID id_call;
        CONST_ID(id_call, "call");
        VALUE rb_callback =
            rb_gi_callback_data_get_rb_callback(data->callback_data);
        data->rb_return_value =
            rb_funcallv(rb_callback,
                        id_call,
                        RARRAY_LENINT(rb_args),
                        RARRAY_CONST_PTR(rb_args));
    }

    return Qnil;
}

static VALUE
rb_gi_callback_invoke_fill_raw_results(VALUE user_data)
{
    RBGICallbackInvokeData *data = (RBGICallbackInvokeData *)user_data;
    rb_gi_arguments_fill_raw_results(data->args,
                                     data->rb_return_value,
                                     data->return_value);
    return Qnil;
}

static VALUE
rb_gi_callback_invoke(VALUE user_data)
{
    RBGICallbackInvokeData *data = (RBGICallbackInvokeData *)user_data;
    int state = 0;
    rb_protect(rb_gi_callback_invoke_without_protect,
               user_data,
               &state);
    if (state != 0) {
        VALUE error = rb_errinfo();
        rb_gi_arguments_fill_raw_out_gerror(data->args, error);
        rb_protect(rb_gi_callback_invoke_fill_raw_results,
                   user_data,
                   &state);
    } else {
        rb_protect(rb_gi_callback_invoke_fill_raw_results,
                   user_data,
                   &state);
        if (state != 0) {
            VALUE error = rb_errinfo();
            rb_gi_arguments_fill_raw_out_gerror(data->args, error);
        }
    }
    return Qnil;
}

static void
rb_gi_ffi_closure_callback(G_GNUC_UNUSED ffi_cif *cif,
                           void *return_value,
                           void **raw_args,
                           void *data)
{
    RBGICallback *callback = data;
    RBGICallbackData *callback_data = NULL;
    RBGIArguments args;

    rb_gi_arguments_init(&args,
                         callback->callback_info,
                         Qnil,
                         Qnil,
                         raw_args);
    {
        guint i;

        for (i = 0; i < args.metadata->len; i++) {
            RBGIArgMetadata *metadata;

            metadata = g_ptr_array_index(args.metadata, i);
            if (!metadata->closure_p) {
                continue;
            }

            callback_data = *((RBGICallbackData **)(raw_args[i]));
            break;
        }

        if (!callback_data && args.metadata->len > 0) {
            RBGIArgMetadata *metadata;

            i = args.metadata->len - 1;
            metadata = g_ptr_array_index(args.metadata, i);
            if (metadata->type.tag == GI_TYPE_TAG_VOID &&
                metadata->type.pointer_p &&
                strcmp(metadata->name, "data") == 0) {
                callback_data = *((RBGICallbackData **)(raw_args[i]));
            }
        }
    }

    {
        RBGICallbackInvokeData data;
        data.args = &args;
        data.callback = callback;
        data.callback_data = callback_data;
        data.return_value = return_value;
        data.rb_return_value = Qnil;
        rbgutil_invoke_callback(rb_gi_callback_invoke, (VALUE)&data);
    }
    rb_gi_arguments_clear(&args);

    if (callback_data) {
        RBGIArgMetadata *callback_metadata =
            rb_gi_callback_data_get_metadata(callback_data);
        if (callback_metadata->scope_type == GI_SCOPE_TYPE_ASYNC) {
            rb_gi_callback_data_free(callback_data);
        }
    }
}

RBGICallback *
rb_gi_callback_new(GICallbackInfo *callback_info,
                   const gchar *method_name)
{
    RBGICallback *callback = RB_ZALLOC(RBGICallback);
    callback->callback_info = callback_info;
    g_base_info_ref(callback->callback_info);
    callback->method_name = g_strdup(method_name);
    callback->closure =
        g_callable_info_prepare_closure(callback->callback_info,
                                        &(callback->cif),
                                        rb_gi_ffi_closure_callback,
                                        callback);
    return callback;
}

static void
rb_gi_callback_free(RBGICallback *callback)
{
    g_callable_info_free_closure(callback->callback_info,
                                 callback->closure);
    g_free(callback->method_name);
    g_base_info_unref(callback->callback_info);
    xfree(callback);
}

static void
rb_gi_callback_data_weak_notify(gpointer data,
                                G_GNUC_UNUSED GObject *where_the_object_was)
{
    RBGICallbackData *callback_data = data;
    callback_data->owner = NULL;
}

RBGICallbackData *
rb_gi_callback_data_new(RBGIArguments *args,
                        RBGICallback *callback,
                        RBGIArgMetadata *metadata)
{
    RBGICallbackData *callback_data;
    VALUE rb_owner = args->rb_receiver;

    if (rb_gi_is_debug_mode()) {
        g_print("[rb-gi] callback: %s::%s()\n",
                g_base_info_get_namespace(args->info),
                g_base_info_get_name(args->info));
        rb_p(rb_ary_new_from_args(2, args->rb_receiver, args->rb_args));
    }

    callback_data = ALLOC(RBGICallbackData);
    callback_data->callback = callback;
    callback_data->metadata = metadata;
    callback_data->rb_callback = rb_block_proc();
    if (NIL_P(rb_owner)) {
        /* Module function case. */
        VALUE rb_first_argument = RARRAY_AREF(args->rb_args, 0);
        if (RVAL2CBOOL(rb_obj_is_kind_of(rb_first_argument, mGLibObject))) {
            /* If the first argument of the module function call is
               GObject, it's suitable for owner.
               For example: pango_cairo_context_set_shape_renderer() */
            rb_owner = rb_first_argument;
        }
    }
    if (RVAL2CBOOL(rb_obj_is_kind_of(rb_owner, mGLibObject))) {
        rbgobj_object_add_relative(rb_owner, callback_data->rb_callback);
        callback_data->owner = RVAL2GOBJ(rb_owner);
        g_object_weak_ref(callback_data->owner,
                          rb_gi_callback_data_weak_notify,
                          callback_data);
        callback_data->rb_owner = Qnil;
    } else {
        /* Callback is GC-ed only when callback is invalidated. */
        if (NIL_P(rb_owner)) {
            /* Module function case. */
            rbgobj_add_relative(mGI, callback_data->rb_callback);
            callback_data->rb_owner = mGI;
        } else {
            /* Class method case. */
            rbgobj_add_relative(rb_owner, callback_data->rb_callback);
            callback_data->rb_owner = rb_owner;
        }
        callback_data->owner = NULL;
    }
    return callback_data;
}

void
rb_gi_callback_data_free(RBGICallbackData *callback_data)
{
    if (callback_data->callback) {
        rb_gi_callback_free(callback_data->callback);
    }
    if (callback_data->owner) {
        VALUE rb_owner;
        g_object_weak_unref(callback_data->owner,
                            rb_gi_callback_data_weak_notify,
                            callback_data);
        rb_owner = rbgobj_ruby_object_from_instance2(callback_data->owner,
                                                     FALSE);
        if (!NIL_P(rb_owner)) {
            rbgobj_object_remove_relative(rb_owner, callback_data->rb_callback);
        }
    }
    if (!NIL_P(callback_data->rb_owner)) {
        rbgobj_remove_relative(callback_data->rb_owner,
                               (ID)0,
                               callback_data->rb_callback);
    }
    xfree(callback_data->metadata);
    xfree(callback_data);
}

RBGIArgMetadata *
rb_gi_callback_data_get_metadata(RBGICallbackData *callback_data)
{
    return callback_data->metadata;
}

VALUE
rb_gi_callback_data_get_rb_callback(RBGICallbackData *callback_data)
{
    return callback_data->rb_callback;
}

void
rb_gi_callback_register_finder(RBGICallbackFinderFunc finder)
{
    g_ptr_array_add(callback_finders, finder);
}

gpointer
rb_gi_callback_find(GIArgInfo *info)
{
    guint i;
    gpointer callback = NULL;

    for (i = 0; i < callback_finders->len; i++) {
        RBGICallbackFinderFunc finder = g_ptr_array_index(callback_finders, i);
        callback = finder(info);
        if (callback) {
            break;
        }
    }

    return callback;
}

static gboolean
source_func_p(GIArgInfo *info)
{
    GITypeInfo type_info;
    GIBaseInfo *interface_info;
    GICallableInfo *callback_info;
    GITypeInfo return_type_info;
    GIArgInfo first_arg_info;
    GITypeInfo first_arg_type_info;

    g_arg_info_load_type(info, &type_info);
    if (g_type_info_get_tag(&type_info) != GI_TYPE_TAG_INTERFACE) {
        return FALSE;
    }

    interface_info = g_type_info_get_interface(&type_info);
    if (g_base_info_get_type(interface_info) != GI_INFO_TYPE_CALLBACK) {
        g_base_info_unref(interface_info);
        return FALSE;
    }

    callback_info = (GICallableInfo *)interface_info;
    g_callable_info_load_return_type(callback_info, &return_type_info);
    if (g_type_info_get_tag(&return_type_info) != GI_TYPE_TAG_BOOLEAN) {
        g_base_info_unref(interface_info);
        return FALSE;
    }

    if (g_callable_info_get_n_args(interface_info) != 1) {
        g_base_info_unref(interface_info);
        return FALSE;
    }

    g_callable_info_load_arg(interface_info, 0, &first_arg_info);
    g_arg_info_load_type(&first_arg_info, &first_arg_type_info);
    if (g_type_info_get_tag(&first_arg_type_info) != GI_TYPE_TAG_VOID) {
        g_base_info_unref(interface_info);
        return FALSE;
    }

    g_base_info_unref(interface_info);
    return TRUE;
}

static gboolean
source_func_callback(gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    VALUE rb_keep;
    ID id_call;

    CONST_ID(id_call, "call");
    rb_keep = rb_funcall(callback_data->rb_callback, id_call, 0);
    if (callback_data->metadata->scope_type == GI_SCOPE_TYPE_ASYNC) {
        rb_gi_callback_data_free(callback_data);
    }
    return RVAL2CBOOL(rb_keep);
}

static gpointer
source_func_callback_finder(GIArgInfo *arg_info)
{
    if (!source_func_p(arg_info)) {
        return NULL;
    }
    return source_func_callback;
}

void
rb_gi_callback_init(VALUE rb_mGI)
{
    callback_finders = g_ptr_array_new();
    rb_gi_callback_register_finder(source_func_callback_finder);

    mGLibObject = rb_const_get(mGLib, rb_intern("Object"));
    mGI = rb_mGI;
}
