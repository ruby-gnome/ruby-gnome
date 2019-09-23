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

static void
rb_gi_callback_data_destroy_notify(gpointer data)
{
    RBGICallbackData *callback_data = data;
    rb_gi_callback_data_free(callback_data);
}

typedef struct {
    RBGIArguments *args;
    RBGICallback *callback;
    RBGICallbackData *callback_data;
} RBGICallbackInvokeData;

static VALUE
rb_gi_callback_invoke(VALUE user_data)
{
    RBGICallbackInvokeData *data = (RBGICallbackInvokeData *)user_data;
    ID id_call;
    VALUE rb_callback = rb_gi_callback_data_get_rb_callback(data->callback_data);
    VALUE rb_args = rb_gi_arguments_get_rb_in_args(data->args);

    CONST_ID(id_call, "call");
    return rb_funcallv(rb_callback,
                       id_call,
                       RARRAY_LENINT(rb_args),
                       RARRAY_CONST_PTR(rb_args));
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
    VALUE rb_results;

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
            if (metadata->type_tag == GI_TYPE_TAG_VOID &&
                metadata->pointer_p &&
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
        rb_results = rbgutil_invoke_callback(rb_gi_callback_invoke,
                                             (VALUE)&data);
    }
    rb_gi_arguments_fill_raw_results(&args, rb_results, return_value);
    rb_gi_arguments_clear(&args);

    {
        RBGIArgMetadata *callback_metadata =
            rb_gi_callback_data_get_metadata(callback_data);
        if (callback_metadata->scope_type == GI_SCOPE_TYPE_ASYNC) {
            rb_gi_callback_data_free(callback_data);
        }
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_callback(RBGIArguments *args,
                                          RBGIArgMetadata *metadata)
{
    gpointer callback_function;
    GIArgInfo *arg_info;
    GIArgument *callback_argument;
    GIArgument *closure_argument = NULL;
    GIArgument *destroy_argument = NULL;
    RBGICallback *callback = NULL;

    arg_info = &(metadata->arg_info);

    callback_argument = &(g_array_index(args->in_args,
                                        GIArgument,
                                        metadata->in_arg_index));
    if (metadata->closure_in_arg_index != -1) {
        closure_argument = &(g_array_index(args->in_args,
                                           GIArgument,
                                           metadata->closure_in_arg_index));
    }
    if (metadata->destroy_in_arg_index != -1) {
        destroy_argument = &(g_array_index(args->in_args,
                                           GIArgument,
                                           metadata->destroy_in_arg_index));
    }

    if (!rb_block_given_p() && g_arg_info_may_be_null(arg_info)) {
        callback_argument->v_pointer = NULL;
        if (closure_argument) {
            closure_argument->v_pointer = NULL;
        }
        if (destroy_argument) {
            destroy_argument->v_pointer = NULL;
        }
        return;
    }

    callback_function = rb_gi_callback_find(arg_info);
    if (callback_function) {
        callback_argument->v_pointer = callback_function;
    } else {
        callback = RB_ZALLOC(RBGICallback);
        callback->type_info = g_arg_info_get_type(arg_info);
        callback->callback_info = g_type_info_get_interface(callback->type_info);
        callback->closure =
            g_callable_info_prepare_closure(callback->callback_info,
                                            &(callback->cif),
                                            rb_gi_ffi_closure_callback,
                                            callback);
        callback_argument->v_pointer = callback->closure;
    }

    if (closure_argument) {
        closure_argument->v_pointer = rb_gi_callback_data_new(args,
                                                              callback,
                                                              metadata);;
    }

    if (destroy_argument) {
        destroy_argument->v_pointer = rb_gi_callback_data_destroy_notify;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby(RBGIArguments *args,
                                 RBGIArgMetadata *metadata)
{
    if (metadata->callback_p && !metadata->destroy_p) {
        rb_gi_arguments_in_init_arg_ruby_callback(args, metadata);
        return;
    }

    if (metadata->rb_arg_index == -1) {
        return;
    }

    if (metadata->array_p) {
        GIArgument *array_argument;
        GIArgument *length_argument = NULL;
        GIArgInfo *length_arg_info = NULL;
        VALUE rb_argument = Qnil;

        if (RARRAY_LEN(args->rb_args) > metadata->rb_arg_index) {
            rb_argument = RARRAY_AREF(args->rb_args, metadata->rb_arg_index);
        }
        array_argument = &(g_array_index(args->in_args,
                                         GIArgument,
                                         metadata->in_arg_index));
        if (metadata->array_length_in_arg_index != -1) {
            RBGIArgMetadata *length_metadata;
            length_argument =
                &(g_array_index(args->in_args,
                                GIArgument,
                                metadata->array_length_in_arg_index));
            length_metadata =
                g_ptr_array_index(args->metadata,
                                  metadata->array_length_arg_index);
            length_arg_info = &(length_metadata->arg_info);
        }
        RVAL2GI_IN_ARRAY_ARGUMENT(array_argument,
                                  length_argument,
                                  &(metadata->arg_info),
                                  length_arg_info,
                                  rb_argument);
    } else {
        GIArgument *argument;
        VALUE rb_argument = Qnil;

        if (RARRAY_LEN(args->rb_args) > metadata->rb_arg_index) {
            rb_argument = RARRAY_AREF(args->rb_args, metadata->rb_arg_index);
        }
        argument = &(g_array_index(args->in_args,
                                   GIArgument,
                                   metadata->in_arg_index));
        rb_gi_in_argument_from_ruby(args->info,
                                    argument,
                                    &(metadata->arg_info),
                                    metadata->rb_arg_index,
                                    rb_argument,
                                    args->rb_receiver);
    }
}

static void
rb_gi_arguments_in_init_arg_raw_interface(RBGIArguments *args,
                                          RBGIArgMetadata *metadata)
{
    GIArgument *argument = &(g_array_index(args->in_args,
                                           GIArgument,
                                           metadata->in_arg_index));
    switch (metadata->interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw argument -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(metadata->interface_type),
                 g_base_info_get_name(metadata->interface_info));
        break;
    case GI_INFO_TYPE_STRUCT:
      argument->v_pointer = *((gpointer *)(args->raw_args[metadata->index]));
      break;
    case GI_INFO_TYPE_BOXED:
    case GI_INFO_TYPE_ENUM:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw argument -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(metadata->interface_type),
                 g_base_info_get_name(metadata->interface_info));
        break;
    case GI_INFO_TYPE_FLAGS:
      argument->v_int32= *((gint32 *)(args->raw_args[metadata->index]));
      break;
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
      argument->v_pointer = *((gpointer *)(args->raw_args[metadata->index]));
      break;
    case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw argument -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(metadata->interface_type),
                 g_base_info_get_name(metadata->interface_info));
        break;
    case GI_INFO_TYPE_INVALID_0:
        g_assert_not_reached();
        break;
    case GI_INFO_TYPE_UNION:
    case GI_INFO_TYPE_VALUE:
    case GI_INFO_TYPE_SIGNAL:
    case GI_INFO_TYPE_VFUNC:
    case GI_INFO_TYPE_PROPERTY:
    case GI_INFO_TYPE_FIELD:
    case GI_INFO_TYPE_ARG:
    case GI_INFO_TYPE_TYPE:
    case GI_INFO_TYPE_UNRESOLVED:
    default:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw argument -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(metadata->interface_type),
                 g_base_info_get_name(metadata->interface_info));
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_raw(RBGIArguments *args,
                                RBGIArgMetadata *metadata)
{
    GIArgument *argument = &(g_array_index(args->in_args,
                                           GIArgument,
                                           metadata->in_arg_index));
    switch (metadata->type_tag) {
      case GI_TYPE_TAG_VOID:
        argument->v_pointer = *((gpointer *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_BOOLEAN:
        argument->v_boolean = *((gboolean *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_INT8:
        argument->v_int8 = *((gint8 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UINT8:
        argument->v_uint8 = *((guint8 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_INT16:
        argument->v_int16 = *((gint16 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UINT16:
        argument->v_uint16 = *((guint16 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_INT32:
        argument->v_int32 = *((gint32 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UINT32:
        argument->v_uint32 = *((guint32 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_INT64:
        argument->v_int64 = *((gint64 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UINT64:
        argument->v_uint64 = *((guint64 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_FLOAT:
        argument->v_float = *((gfloat *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_DOUBLE:
        argument->v_double = *((gdouble *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_GTYPE:
        argument->v_size = *((gsize *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        argument->v_string = *((gchar **)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_ARRAY:
        argument->v_pointer = *((gpointer *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_in_init_arg_raw_interface(args, metadata);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        argument->v_pointer = *((gpointer *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_ERROR:
        argument->v_pointer = *((GError **)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UNICHAR:
        argument->v_uint32 = *((gunichar *)(args->raw_args[metadata->index]));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

void
rb_gi_arguments_in_init(RBGIArguments *args)
{
    guint i;

    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;

        metadata = g_ptr_array_index(args->metadata, i);
        if (metadata->direction != GI_DIRECTION_IN) {
            continue;
        }

        if (args->rb_mode_p) {
            rb_gi_arguments_in_init_arg_ruby(args, metadata);
        } else {
            rb_gi_arguments_in_init_arg_raw(args, metadata);
        }
    }
}

void
rb_gi_arguments_in_clear(RBGIArguments *args)
{
    guint i;

    if (!args->rb_mode_p) {
        return;
    }

    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;
        gint in_arg_index;
        gint rb_arg_index;
        VALUE rb_argument = Qnil;
        GIArgument *argument;

        metadata = g_ptr_array_index(args->metadata, i);
        if (metadata->direction == GI_DIRECTION_OUT) {
            continue;
        }

        in_arg_index = metadata->in_arg_index;
        if (in_arg_index == -1) {
            continue;
        }

        rb_arg_index = metadata->rb_arg_index;
        if (RARRAY_LEN(args->rb_args) > rb_arg_index) {
            rb_argument = RARRAY_AREF(args->rb_args, rb_arg_index);
        }
        argument = &(g_array_index(args->in_args,
                                   GIArgument,
                                   in_arg_index));
        rb_gi_in_argument_free(rb_argument,
                               argument,
                               &(metadata->arg_info));
    }
}
