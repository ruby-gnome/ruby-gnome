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
rb_gi_arguments_allocate(RBGIArguments *args)
{
    gint i, n_args;

    n_args = g_callable_info_get_n_args(args->info);
    for (i = 0; i < n_args; i++) {
        GIArgument argument = {0};
        RBGIArgMetadata *metadata;
        GIArgInfo *arg_info;
        GIDirection direction;

        metadata = ALLOC(RBGIArgMetadata);
        metadata->callable_info = args->info;
        arg_info = &(metadata->arg_info);
        g_callable_info_load_arg(args->info, i, arg_info);
        metadata->scope_type = g_arg_info_get_scope(arg_info);
        metadata->direction = g_arg_info_get_direction(arg_info);
        metadata->callback_p = (metadata->scope_type != GI_SCOPE_TYPE_INVALID);
        metadata->closure_p = FALSE;
        metadata->destroy_p = FALSE;
        metadata->array_p = FALSE;
        metadata->array_length_p = FALSE;
        metadata->in_arg_index = -1;
        metadata->closure_in_arg_index = -1;
        metadata->destroy_in_arg_index = -1;
        metadata->array_in_arg_index = -1;
        metadata->array_length_in_arg_index = -1;
        metadata->array_length_arg_index = -1;
        metadata->rb_arg_index = -1;
        metadata->out_arg_index = -1;

        direction = metadata->direction;
        if (direction == GI_DIRECTION_IN || direction == GI_DIRECTION_INOUT) {
            metadata->in_arg_index = args->in_args->len;
            g_array_append_val(args->in_args, argument);
        }
        if (direction == GI_DIRECTION_OUT || direction == GI_DIRECTION_INOUT) {
            metadata->out_arg_index = args->out_args->len;
            g_array_append_val(args->out_args, argument);
        }

        g_ptr_array_add(args->metadata, metadata);
    }
}

static void
rb_gi_arguments_fill_metadata_callback(RBGIArguments *args)
{
    GPtrArray *metadata = args->metadata;
    guint i;

    for (i = 0; i < metadata->len; i++) {
        RBGIArgMetadata *callback_metadata;
        GIArgInfo *arg_info;
        gint closure_index;
        gint destroy_index;

        callback_metadata = g_ptr_array_index(metadata, i);

        arg_info = &(callback_metadata->arg_info);
        closure_index = g_arg_info_get_closure(arg_info);
        if (closure_index != -1) {
            RBGIArgMetadata *closure_metadata;
            closure_metadata = g_ptr_array_index(metadata, closure_index);
            closure_metadata->closure_p = TRUE;
            callback_metadata->closure_in_arg_index =
                closure_metadata->in_arg_index;
        }

        destroy_index = g_arg_info_get_destroy(arg_info);
        if (destroy_index != -1) {
            RBGIArgMetadata *destroy_metadata;
            destroy_metadata = g_ptr_array_index(args->metadata, destroy_index);
            destroy_metadata->destroy_p = TRUE;
            callback_metadata->destroy_in_arg_index =
                destroy_metadata->in_arg_index;
        }
    }
}

static void
rb_gi_arguments_fill_metadata_array(RBGIArguments *args)
{
    GPtrArray *metadata = args->metadata;
    guint i;

    for (i = 0; i < metadata->len; i++) {
        RBGIArgMetadata *array_metadata;
        RBGIArgMetadata *array_length_metadata;
        GIArgInfo *arg_info;
        GITypeInfo type_info;
        gint array_length_index = -1;

        array_metadata = g_ptr_array_index(metadata, i);
        arg_info = &(array_metadata->arg_info);

        g_arg_info_load_type(arg_info, &type_info);
        if (g_type_info_get_tag(&type_info) != GI_TYPE_TAG_ARRAY) {
            continue;
        }
        array_metadata->array_p = TRUE;

        array_length_index = g_type_info_get_array_length(&type_info);
        if (array_length_index == -1) {
            continue;
        }

        array_length_metadata = g_ptr_array_index(metadata, array_length_index);
        array_length_metadata->array_length_p = TRUE;
        array_length_metadata->rb_arg_index = -1;
        array_length_metadata->array_in_arg_index =
            array_metadata->in_arg_index;
        array_metadata->array_length_in_arg_index =
            array_length_metadata->in_arg_index;
        array_metadata->array_length_arg_index = array_length_index;
    }
}

static void
rb_gi_arguments_fill_metadata_array_from_callable_info(RBGIArguments *args)
{
    GICallbackInfo *info = args->info;
    GPtrArray *metadata = args->metadata;
    GITypeInfo return_type_info;
    RBGIArgMetadata *array_length_metadata;
    gint array_length_index = -1;

    g_callable_info_load_return_type(info, &return_type_info);
    if (g_type_info_get_tag(&return_type_info) != GI_TYPE_TAG_ARRAY) {
        return;
    }

    array_length_index = g_type_info_get_array_length(&return_type_info);
    if (array_length_index == -1) {
        return;
    }

    array_length_metadata = g_ptr_array_index(metadata, array_length_index);
    array_length_metadata->array_length_p = TRUE;
    array_length_metadata->rb_arg_index = -1;
}

static void
rb_gi_arguments_fill_metadata_rb_arg_index(RBGIArguments *args)
{
    guint i;
    gint rb_arg_index = 0;

    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;

        metadata = g_ptr_array_index(args->metadata, i);

        if (metadata->callback_p) {
            continue;
        }

        if (metadata->closure_p) {
            continue;
        }

        if (metadata->destroy_p) {
            continue;
        }

        if (metadata->array_length_p) {
            continue;
        }

        if (metadata->in_arg_index == -1) {
            continue;
        }

        metadata->rb_arg_index = rb_arg_index;
        rb_arg_index++;
    }
}

static void
rb_gi_arguments_fill_metadata(RBGIArguments *args)
{
    rb_gi_arguments_fill_metadata_callback(args);
    rb_gi_arguments_fill_metadata_array(args);
    rb_gi_arguments_fill_metadata_array_from_callable_info(args);
    rb_gi_arguments_fill_metadata_rb_arg_index(args);
}

static void
rb_gi_callback_data_destroy_notify(gpointer data)
{
    RBGICallbackData *callback_data = data;
    rb_gi_callback_data_free(callback_data);
}

static void
ffi_closure_callback(G_GNUC_UNUSED ffi_cif *cif,
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
            GIArgInfo *arg_info;
            GITypeInfo *type_info;
            GITypeTag type_tag;
            gboolean is_pointer;
            const gchar *arg_name;

            i = args.metadata->len - 1;
            metadata = g_ptr_array_index(args.metadata, i);
            arg_info = &(metadata->arg_info);
            type_info = g_arg_info_get_type(arg_info);
            type_tag = g_type_info_get_tag(type_info);
            is_pointer = g_type_info_is_pointer(type_info);
            g_base_info_unref(type_info);
            arg_name = g_base_info_get_name(arg_info);
            if (type_tag == GI_TYPE_TAG_VOID &&
                is_pointer &&
                strcmp(arg_name, "data") == 0) {
                callback_data = *((RBGICallbackData **)(raw_args[i]));
            }
        }
    }

    {
        ID id_call;
        VALUE rb_callback = rb_gi_callback_data_get_rb_callback(callback_data);
        VALUE rb_args = rb_gi_arguments_get_rb_in_args(&args);
        CONST_ID(id_call, "call");
        /* TODO: use rb_protect() */
        rb_results = rb_funcallv(rb_callback,
                                 id_call,
                                 RARRAY_LENINT(rb_args),
                                 RARRAY_CONST_PTR(rb_args));
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
rb_gi_arguments_fill_input_rb_args_callback(RBGIArguments *args,
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
                                            ffi_closure_callback,
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
rb_gi_arguments_fill_input_rb_args(RBGIArguments *args,
                                   RBGIArgMetadata *metadata)
{
    if (metadata->callback_p && !metadata->destroy_p) {
        rb_gi_arguments_fill_input_rb_args_callback(args, metadata);
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
rb_gi_arguments_fill_output_rb_args(RBGIArguments *args,
                                    RBGIArgMetadata *metadata)
{
    GIArgument *argument;

    argument = &(g_array_index(args->out_args,
                               GIArgument,
                               metadata->out_arg_index));
    rb_gi_out_argument_init(argument, &(metadata->arg_info));
}

static void
rb_gi_arguments_fill_rb_args(RBGIArguments *args)
{
    gint i, n_args;

    n_args = g_callable_info_get_n_args(args->info);
    for (i = 0; i < n_args; i++) {
        RBGIArgMetadata *metadata;

        metadata = g_ptr_array_index(args->metadata, i);
        if (metadata->in_arg_index != -1) {
            rb_gi_arguments_fill_input_rb_args(args, metadata);
        } else {
            rb_gi_arguments_fill_output_rb_args(args, metadata);
        }
    }
}

static void
rb_gi_arguments_fill_input_raw_args_interface(RBGIArguments *args,
                                              guint i,
                                              GIArgument *argument,
                                              GITypeInfo *type_info)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw argument -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_STRUCT:
      argument->v_pointer = *((gpointer *)(args->raw_args[i]));
      break;
    case GI_INFO_TYPE_BOXED:
    case GI_INFO_TYPE_ENUM:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw argument -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_FLAGS:
      argument->v_int32= *((gint32 *)(args->raw_args[i]));
      break;
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
      argument->v_pointer = *((gpointer *)(args->raw_args[i]));
      break;
    case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw argument -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
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
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    }

    g_base_info_unref(interface_info);
}

static void
rb_gi_arguments_fill_raw_args(RBGIArguments *args)
{
    guint i;

    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;
        GIArgument *argument;
        GITypeInfo *type_info;
        GITypeTag type_tag;

        metadata = g_ptr_array_index(args->metadata, i);

        if (metadata->direction == GI_DIRECTION_INOUT) {
            argument = &g_array_index(args->in_args,
                                      GIArgument,
                                      metadata->in_arg_index);
            argument->v_pointer = *((gpointer *)(args->raw_args[i]));
            return;
        } else if (metadata->direction == GI_DIRECTION_OUT) {
            argument = &g_array_index(args->out_args,
                                      GIArgument,
                                      metadata->out_arg_index);
            argument->v_pointer = *((gpointer *)(args->raw_args[i]));
            return;
        }

        argument = &g_array_index(args->in_args,
                                  GIArgument,
                                  metadata->in_arg_index);
        type_info = g_arg_info_get_type(&(metadata->arg_info));
        type_tag = g_type_info_get_tag(type_info);

        switch (type_tag) {
          case GI_TYPE_TAG_VOID:
            argument->v_pointer = *((gpointer *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_BOOLEAN:
            argument->v_boolean = *((gboolean *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_INT8:
            argument->v_int8 = *((gint8 *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_UINT8:
            argument->v_uint8 = *((guint8 *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_INT16:
            argument->v_int16 = *((gint16 *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_UINT16:
            argument->v_uint16 = *((guint16 *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_INT32:
            argument->v_int32 = *((gint32 *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_UINT32:
            argument->v_uint32 = *((guint32 *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_INT64:
            argument->v_int64 = *((gint64 *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_UINT64:
            argument->v_uint64 = *((guint64 *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_FLOAT:
            argument->v_float = *((gfloat *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_DOUBLE:
            argument->v_double = *((gdouble *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_GTYPE:
            argument->v_size = *((gsize *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_UTF8:
          case GI_TYPE_TAG_FILENAME:
            argument->v_string = *((gchar **)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_ARRAY:
            argument->v_pointer = *((gpointer *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_INTERFACE:
            rb_gi_arguments_fill_input_raw_args_interface(args,
                                                          i,
                                                          argument,
                                                          type_info);
            break;
          case GI_TYPE_TAG_GLIST:
          case GI_TYPE_TAG_GSLIST:
          case GI_TYPE_TAG_GHASH:
            argument->v_pointer = *((gpointer *)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_ERROR:
            argument->v_pointer = *((GError **)(args->raw_args[i]));
            break;
          case GI_TYPE_TAG_UNICHAR:
            argument->v_uint32 = *((gunichar *)(args->raw_args[i]));
            break;
          default:
            g_assert_not_reached();
            break;
        }

        g_base_info_unref(type_info);
    }
}

static void
rb_gi_arguments_metadata_free(gpointer data)
{
    RBGIArgMetadata *metadata = data;
    if (metadata->scope_type == GI_SCOPE_TYPE_ASYNC ||
        metadata->scope_type == GI_SCOPE_TYPE_NOTIFIED) {
        return;
    }
    xfree(metadata);
}

static gboolean
rb_gi_arguments_gobject_based_p(RBGIArguments *args)
{
    GIBaseInfo *container_info;
    GIRegisteredTypeInfo *registered_type_info;

    container_info = g_base_info_get_container(args->info);
    if (g_base_info_get_type(container_info) != GI_INFO_TYPE_STRUCT) {
        return TRUE;
    }

    registered_type_info = (GIRegisteredTypeInfo *)container_info;
    if (g_registered_type_info_get_type_init(registered_type_info)) {
        return TRUE;
    }

    return FALSE;
}

void
rb_gi_arguments_init(RBGIArguments *args,
                     GICallableInfo *info,
                     VALUE rb_receiver,
                     VALUE rb_args,
                     void **raw_args)
{
    args->info = info;
    args->rb_receiver = rb_receiver;
    args->receiver_type_class = NULL;
    args->rb_args = rb_args;
    args->raw_args = raw_args;
    args->in_args = g_array_new(FALSE, FALSE, sizeof(GIArgument));
    args->out_args = g_array_new(FALSE, FALSE, sizeof(GIArgument));
    args->metadata =
        g_ptr_array_new_with_free_func(rb_gi_arguments_metadata_free);

    if (!NIL_P(rb_receiver)) {
        GIArgument receiver;
        VALUE rb_receiver_class;
        rb_receiver_class = rb_class_of(rb_receiver);
        if (rb_gi_arguments_gobject_based_p(args) ||
            rb_respond_to(rb_receiver_class, rb_intern("gtype"))) {
            receiver.v_pointer = RVAL2GOBJ(rb_receiver);
        } else if (RVAL2CBOOL(rb_obj_is_kind_of(rb_receiver, rb_cClass)) &&
                   rb_respond_to(rb_receiver, rb_intern("gtype"))) {
            args->receiver_type_class =
                g_type_class_ref(CLASS2GTYPE(rb_receiver));
            receiver.v_pointer = args->receiver_type_class;
        } else {
            receiver.v_pointer = rb_gi_struct_get_raw(rb_receiver, G_TYPE_NONE);
        }
        if (receiver.v_pointer) {
            g_array_append_val(args->in_args, receiver);
        }
    }

    rb_gi_arguments_allocate(args);
    rb_gi_arguments_fill_metadata(args);
    if (!NIL_P(rb_args)) {
        rb_gi_arguments_fill_rb_args(args);
    } else {
        rb_gi_arguments_fill_raw_args(args);
    }
}

void
rb_gi_arguments_clear(RBGIArguments *args)
{
    guint i;

    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;
        gint in_arg_index;

        metadata = g_ptr_array_index(args->metadata, i);
        if (metadata->direction == GI_DIRECTION_IN ||
            metadata->direction == GI_DIRECTION_INOUT) {
            in_arg_index = metadata->in_arg_index;
            if (in_arg_index != -1) {
                gint rb_arg_index;
                VALUE rb_argument = Qnil;
                GIArgument *argument;

                rb_arg_index = metadata->rb_arg_index;
                if (RARRAY_LEN(args->rb_args) > rb_arg_index) {
                    rb_argument = RARRAY_PTR(args->rb_args)[rb_arg_index];
                }
                argument = &(g_array_index(args->in_args,
                                           GIArgument,
                                           in_arg_index));
                rb_gi_in_argument_free(rb_argument,
                                       argument,
                                       &(metadata->arg_info));
            }
        } else {
            GIArgument *argument;
            argument = &(g_array_index(args->out_args,
                                       GIArgument,
                                       metadata->out_arg_index));
            rb_gi_out_argument_fin(argument, &(metadata->arg_info));
        }
    }

    if (args->receiver_type_class) {
        g_type_class_unref(args->receiver_type_class);
    }
    g_array_unref(args->in_args);
    g_array_unref(args->out_args);
    g_ptr_array_unref(args->metadata);
}

VALUE
rb_gi_arguments_get_rb_return_value(RBGIArguments *args,
                                    GIArgument *return_value)
{
    /* TODO */
    VALUE rb_return_value = GI_RETURN_ARGUMENT2RVAL(args->info,
                                                    return_value,
                                                    args->in_args,
                                                    args->out_args,
                                                    args->metadata);
    return rb_return_value;
}

VALUE
rb_gi_arguments_get_rb_out_args(RBGIArguments *args)
{
    gint i, n_args;
    VALUE rb_out_args;

    rb_out_args = rb_ary_new();
    n_args = g_callable_info_get_n_args(args->info);
    for (i = 0; i < n_args; i++) {
        RBGIArgMetadata *metadata;
        GIArgument *argument = NULL;
        VALUE rb_argument;

        metadata = g_ptr_array_index(args->metadata, i);
        if (metadata->array_length_p) {
            continue;
        }

        switch (metadata->direction) {
          case GI_DIRECTION_IN:
            break;
          case GI_DIRECTION_OUT:
            argument = &g_array_index(args->out_args,
                                      GIArgument,
                                      metadata->out_arg_index);
            break;
          case GI_DIRECTION_INOUT:
            argument = &g_array_index(args->in_args,
                                      GIArgument,
                                      metadata->in_arg_index);
            break;
          default:
            g_assert_not_reached();
            break;
        }

        if (!argument) {
            continue;
        }

        rb_argument = GI_OUT_ARGUMENT2RVAL(argument,
                                           &(metadata->arg_info),
                                           args->in_args,
                                           args->out_args,
                                           args->metadata);
        rb_ary_push(rb_out_args, rb_argument);
    }

    if (RARRAY_LEN(rb_out_args) == 0) {
        return Qnil;
    } else {
        return rb_out_args;
    }
}

static void
rb_gi_arguments_fill_raw_result_interface(RBGIArguments *args,
                                          GIArgument *argument,
                                          gpointer raw_result,
                                          GITypeInfo *type_info,
                                          G_GNUC_UNUSED GITransfer transfer /* TODO */,
                                          gboolean is_return_value)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GIFFIReturnValue *ffi_return_value = raw_result;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
    case GI_INFO_TYPE_STRUCT:
    case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw result(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_ENUM:
      if (is_return_value) {
          ffi_return_value->v_ulong = argument->v_int;
      } else {
          *((gint *)raw_result) = argument->v_int;
      }
      break;
    case GI_INFO_TYPE_FLAGS:
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
    case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw result(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
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
                 "TODO: %s::%s: out raw result(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    }

    g_base_info_unref(interface_info);
}

/*
  We need to cast from different type for return value. (We don't
  need it for out arguments.) Because of libffi specification:

  https://github.com/libffi/libffi/blob/master/doc/libffi.texi#L190

  @var{rvalue} is a pointer to a chunk of memory that will hold the
  result of the function call.  This must be large enough to hold the
  result, no smaller than the system register size (generally 32 or 64
  bits), and must be suitably aligned; it is the caller's responsibility
  to ensure this.  If @var{cif} declares that the function returns
  @code{void} (using @code{ffi_type_void}), then @var{rvalue} is
  ignored.

  https://github.com/libffi/libffi/blob/master/doc/libffi.texi#L198

  In most situations, @samp{libffi} will handle promotion according to
  the ABI.  However, for historical reasons, there is a special case
  with return values that must be handled by your code.  In particular,
  for integral (not @code{struct}) types that are narrower than the
  system register size, the return value will be widened by
  @samp{libffi}.  @samp{libffi} provides a type, @code{ffi_arg}, that
  can be used as the return type.  For example, if the CIF was defined
  with a return type of @code{char}, @samp{libffi} will try to store a
  full @code{ffi_arg} into the return value.

  See also:
    * https://github.com/ruby-gnome/ruby-gnome/issues/758#issuecomment-243149237
    * https://github.com/libffi/libffi/pull/216

  This ffi_return_value case implementation is based on
  gi_type_info_extract_ffi_return_value().
*/
static void
rb_gi_arguments_fill_raw_result(RBGIArguments *args,
                                VALUE rb_result,
                                gpointer raw_result,
                                GITypeInfo *type_info,
                                GITransfer transfer,
                                gboolean is_return_value)
{
    GIArgument argument;
    GITypeTag type_tag;
    GIFFIReturnValue *ffi_return_value = raw_result;

    rb_gi_value_argument_from_ruby(&argument,
                                   type_info,
                                   rb_result,
                                   rb_result);
    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
        g_assert_not_reached();
        break;
      case GI_TYPE_TAG_BOOLEAN:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_boolean;
        } else {
            *((gboolean *)raw_result) = argument.v_boolean;
        }
        break;
      case GI_TYPE_TAG_INT8:
        if (is_return_value) {
            ffi_return_value->v_long = argument.v_int8;
        } else {
            *((gint8 *)raw_result) = argument.v_int8;
        }
        break;
      case GI_TYPE_TAG_UINT8:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_uint8;
        } else {
            *((guint8 *)raw_result) = argument.v_uint8;
        }
        break;
      case GI_TYPE_TAG_INT16:
        if (is_return_value) {
            ffi_return_value->v_long = argument.v_int16;
        } else {
            *((gint16 *)raw_result) = argument.v_int16;
        }
        break;
      case GI_TYPE_TAG_UINT16:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_uint16;
        } else {
            *((guint16 *)raw_result) = argument.v_uint16;
        }
        break;
      case GI_TYPE_TAG_INT32:
        if (is_return_value) {
            ffi_return_value->v_long = argument.v_int32;
        } else {
            *((gint32 *)raw_result) = argument.v_int32;
        }
        break;
      case GI_TYPE_TAG_UINT32:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_uint32;
        } else {
            *((guint32 *)raw_result) = argument.v_uint32;
        }
        break;
      case GI_TYPE_TAG_INT64:
        *((gint64 *)raw_result) = argument.v_int64;
        break;
      case GI_TYPE_TAG_UINT64:
        *((guint64 *)raw_result) = argument.v_uint64;
        break;
      case GI_TYPE_TAG_FLOAT:
        *((gfloat *)raw_result) = argument.v_float;
        break;
      case GI_TYPE_TAG_DOUBLE:
        *((gdouble *)raw_result) = argument.v_double;
        break;
      case GI_TYPE_TAG_GTYPE:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_size;
        } else {
            *((gsize *)raw_result) = argument.v_size;
        }
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        if (is_return_value) {
            ffi_return_value->v_ulong = (gulong)(argument.v_string);
        } else {
            *((gchar **)raw_result) = argument.v_string;
        }
        break;
      case GI_TYPE_TAG_ARRAY:
        if (is_return_value) {
            ffi_return_value->v_ulong = (gulong)(argument.v_pointer);
        } else {
            *((gpointer *)raw_result) = argument.v_pointer;
        }
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_fill_raw_result_interface(args,
                                                  &argument,
                                                  raw_result,
                                                  type_info,
                                                  transfer,
                                                  is_return_value);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        if (is_return_value) {
            ffi_return_value->v_ulong = (gulong)(argument.v_pointer);
        } else {
            *((gpointer *)raw_result) = argument.v_pointer;
        }
        break;
      case GI_TYPE_TAG_ERROR:
        if (is_return_value) {
            ffi_return_value->v_ulong = (gulong)(argument.v_pointer);
        } else {
            *((GError **)raw_result) = argument.v_pointer;
        }
        break;
      case GI_TYPE_TAG_UNICHAR:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_uint32;
        } else {
            *((gunichar *)raw_result) = argument.v_uint32;
        }
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

void
rb_gi_arguments_fill_raw_results(RBGIArguments *args,
                                 VALUE rb_results,
                                 gpointer raw_return_value)
{
    int i_rb_result = 0;
    guint i;
    GITypeInfo *return_type_info;
    GITypeTag return_type_tag;

    return_type_info = g_callable_info_get_return_type(args->info);
    return_type_tag = g_type_info_get_tag(return_type_info);
    if (return_type_tag != GI_TYPE_TAG_VOID) {
        GITransfer transfer;
        transfer = g_callable_info_get_caller_owns(args->info);
        if (args->out_args->len == 0) {
            VALUE rb_return_value = rb_results;
            rb_gi_arguments_fill_raw_result(args,
                                            rb_return_value,
                                            raw_return_value,
                                            return_type_info,
                                            transfer,
                                            TRUE);
        } else {
            rb_gi_arguments_fill_raw_result(args,
                                            RARRAY_AREF(rb_results, i_rb_result),
                                            raw_return_value,
                                            return_type_info,
                                            transfer,
                                            TRUE);
            i_rb_result++;
        }
    }
    g_base_info_unref(return_type_info);

    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;
        GIArgument *argument;
        GITypeInfo *type_info;
        GITransfer transfer;

        metadata = g_ptr_array_index(args->metadata, i);

        /* TODO: support GI_DIRECTION_INOUT */
        if (metadata->direction != GI_DIRECTION_OUT) {
            continue;
        }

        argument = &g_array_index(args->out_args,
                                  GIArgument,
                                  metadata->out_arg_index);
        type_info = g_arg_info_get_type(&(metadata->arg_info));
        transfer = g_arg_info_get_ownership_transfer(&(metadata->arg_info));
        rb_gi_arguments_fill_raw_result(args,
                                        RARRAY_AREF(rb_results, i_rb_result),
                                        argument->v_pointer,
                                        return_type_info,
                                        transfer,
                                        FALSE);
        i_rb_result++;
        g_base_info_unref(type_info);
    }
}

VALUE
rb_gi_arguments_get_rb_in_args(RBGIArguments *args)
{
    VALUE rb_args;
    guint i;

    rb_args = rb_ary_new_capa(args->metadata->len);
    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;
        GIArgument *argument;
        GITypeInfo *type_info;
        VALUE rb_arg;

        metadata = g_ptr_array_index(args->metadata, i);

        if (metadata->direction == GI_DIRECTION_OUT) {
            continue;
        }
        if (metadata->closure_p) {
            continue;
        }

        argument = &g_array_index(args->in_args,
                                  GIArgument,
                                  metadata->in_arg_index);
        type_info = g_arg_info_get_type(&(metadata->arg_info));
        /* TODO */
        rb_arg = GI_ARGUMENT2RVAL(argument,
                                  FALSE,
                                  type_info,
                                  args->in_args,
                                  args->out_args,
                                  args->metadata);
        rb_ary_push(rb_args, rb_arg);
    }

    return rb_args;
}
