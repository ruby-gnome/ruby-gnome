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
rb_gi_arguments_in_to_ruby(RBGIArguments *args)
{
    VALUE rb_in_args;
    guint i;

    rb_in_args = rb_ary_new_capa(args->metadata->len);
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
        rb_ary_push(rb_in_args, rb_arg);
    }

    return rb_in_args;
}

static VALUE
rb_gi_callback_invoke(VALUE user_data)
{
    RBGICallbackInvokeData *data = (RBGICallbackInvokeData *)user_data;
    ID id_call;
    VALUE rb_callback = rb_gi_callback_data_get_rb_callback(data->callback_data);
    VALUE rb_args = rb_gi_arguments_in_to_ruby(data->args);

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
                                                              metadata);
    }

    if (destroy_argument) {
        destroy_argument->v_pointer = rb_gi_callback_data_destroy_notify;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_transfer_interface(RBGIArguments *args,
                                                    RBGIArgMetadata *metadata,
                                                    VALUE rb_argument)
{
    GIArgument *argument;
    argument = &(g_array_index(args->in_args,
                               GIArgument,
                               metadata->in_arg_index));
    switch (metadata->interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: %s transfer (interface)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_INFO_TYPE_STRUCT:
        rbgobj_boxed_unown(rb_argument);
        break;
      case GI_INFO_TYPE_BOXED:
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: %s transfer (interface)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_INFO_TYPE_OBJECT:
        g_object_unref(argument->v_pointer);
        break;
      case GI_INFO_TYPE_INTERFACE:
      case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: %s transfer (interface)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
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
        rb_raise(rb_eNotImpError,
                 "TODO: %s transfer (interface)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_transfer(RBGIArguments *args,
                                          RBGIArgMetadata *metadata,
                                          VALUE rb_argument)
{
    if (metadata->transfer == GI_TRANSFER_NOTHING) {
        return;
    }

    /* TODO */
    if (metadata->direction == GI_DIRECTION_INOUT) {
        return;
    }

    switch (metadata->type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
      case GI_TYPE_TAG_UINT8:
      case GI_TYPE_TAG_INT16:
      case GI_TYPE_TAG_UINT16:
      case GI_TYPE_TAG_INT32:
      case GI_TYPE_TAG_UINT32:
      case GI_TYPE_TAG_INT64:
      case GI_TYPE_TAG_UINT64:
      case GI_TYPE_TAG_FLOAT:
      case GI_TYPE_TAG_DOUBLE:
      case GI_TYPE_TAG_GTYPE:
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: %s transfer (%s)[%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_in_init_arg_ruby_transfer_interface(args,
                                                            metadata,
                                                            rb_argument);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: %s transfer (%s)[%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_array_set_length(RBGIArguments *args,
                                                  RBGIArgMetadata *metadata,
                                                  gint64 length)
{
    GIArgument *length_argument;
    RBGIArgMetadata *length_metadata;

    if (metadata->array_length_in_arg_index == -1) {
        return;
    }

    length_argument = &(g_array_index(args->in_args,
                                      GIArgument,
                                      metadata->array_length_in_arg_index));
    length_metadata =
        g_ptr_array_index(args->metadata,
                          metadata->array_length_arg_index);

    switch (length_metadata->type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid argument?: length[%s]",
                 g_type_tag_to_string(length_metadata->type_tag));
        break;
      case GI_TYPE_TAG_INT8:
        if (metadata->direction == GI_DIRECTION_INOUT) {
            length_argument->v_pointer = ALLOC(gint8);
            *((gint8 *)length_argument->v_pointer) = length;
        } else {
            length_argument->v_int8 = length;
        }
        break;
      case GI_TYPE_TAG_UINT8:
        if (metadata->direction == GI_DIRECTION_INOUT) {
            length_argument->v_pointer = ALLOC(guint8);
            *((guint8 *)length_argument->v_pointer) = length;
        } else {
            length_argument->v_uint8 = length;
        }
        break;
      case GI_TYPE_TAG_INT16:
        if (metadata->direction == GI_DIRECTION_INOUT) {
            length_argument->v_pointer = ALLOC(gint16);
            *((gint16 *)length_argument->v_pointer) = length;
        } else {
            length_argument->v_int16 = length;
        }
        break;
      case GI_TYPE_TAG_UINT16:
        if (metadata->direction == GI_DIRECTION_INOUT) {
            length_argument->v_pointer = ALLOC(guint16);
            *((guint16 *)length_argument->v_pointer) = length;
        } else {
            length_argument->v_uint16 = length;
        }
        break;
      case GI_TYPE_TAG_INT32:
        if (metadata->direction == GI_DIRECTION_INOUT) {
            length_argument->v_pointer = ALLOC(gint32);
            *((gint32 *)length_argument->v_pointer) = (gint32)length;
        } else {
            length_argument->v_int32 = (gint32)length;
        }
        break;
      case GI_TYPE_TAG_UINT32:
        if (metadata->direction == GI_DIRECTION_INOUT) {
            length_argument->v_pointer = ALLOC(guint32);
            *((guint32 *)length_argument->v_pointer) = (guint32)length;
        } else {
            length_argument->v_uint32 = (guint32)length;
        }
        break;
      case GI_TYPE_TAG_INT64:
        if (metadata->direction == GI_DIRECTION_INOUT) {
            length_argument->v_pointer = ALLOC(gint64);
            *((gint64 *)length_argument->v_pointer) = length;
        } else {
            length_argument->v_int64 = length;
        }
        break;
      case GI_TYPE_TAG_UINT64:
        if (metadata->direction == GI_DIRECTION_INOUT) {
            length_argument->v_pointer = ALLOC(guint64);
            *((guint64 *)length_argument->v_pointer) = length;
        } else {
            length_argument->v_uint64 = length;
        }
        break;
      case GI_TYPE_TAG_FLOAT:
      case GI_TYPE_TAG_DOUBLE:
      case GI_TYPE_TAG_GTYPE:
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
      case GI_TYPE_TAG_INTERFACE:
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid argument?: length[%s]",
                 g_type_tag_to_string(length_metadata->type_tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_generic(RBGIArguments *args,
                                                 RBGIArgMetadata *metadata,
                                                 VALUE rb_array,
                                                 gpointer raw_array)
{
    GIArgument *array_argument;
    long n_elements;

    array_argument = &(g_array_index(args->in_args,
                                     GIArgument,
                                     metadata->in_arg_index));
    if (metadata->direction == GI_DIRECTION_INOUT) {
        array_argument->v_pointer = ALLOC(gpointer);
        *((gpointer *)(array_argument->v_pointer)) = raw_array;
    } else {
        array_argument->v_pointer = raw_array;
    }

    n_elements = RARRAY_LEN(rb_array);
    rb_gi_arguments_in_init_arg_ruby_array_set_length(args,
                                                      metadata,
                                                      n_elements);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_boolean(RBGIArguments *args,
                                                 RBGIArgMetadata *metadata,
                                                 VALUE rb_array)
{
    gboolean *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(gboolean, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = RVAL2CBOOL(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_int8(RBGIArguments *args,
                                              RBGIArgMetadata *metadata,
                                              VALUE rb_array)
{
    gint8 *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(gint8, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = NUM2CHR(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_uint8(RBGIArguments *args,
                                               RBGIArgMetadata *metadata,
                                               VALUE rb_array)
{
    guint8 *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(guint8, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = (guint8)NUM2CHR(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_int16(RBGIArguments *args,
                                               RBGIArgMetadata *metadata,
                                               VALUE rb_array)
{
    gint16 *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(gint16, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = NUM2SHORT(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_uint16(RBGIArguments *args,
                                                RBGIArgMetadata *metadata,
                                                VALUE rb_array)
{
    guint16 *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(guint16, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = NUM2USHORT(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_int32(RBGIArguments *args,
                                               RBGIArgMetadata *metadata,
                                               VALUE rb_array)
{
    gint32 *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(gint32, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = NUM2INT(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_uint32(RBGIArguments *args,
                                                RBGIArgMetadata *metadata,
                                                VALUE rb_array)
{
    guint32 *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(guint32, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = NUM2UINT(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_int64(RBGIArguments *args,
                                               RBGIArgMetadata *metadata,
                                               VALUE rb_array)
{
    gint64 *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(gint64, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = NUM2LL(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_uint64(RBGIArguments *args,
                                                RBGIArgMetadata *metadata,
                                                VALUE rb_array)
{
    guint64 *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(guint64, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = NUM2ULL(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_float(RBGIArguments *args,
                                               RBGIArgMetadata *metadata,
                                               VALUE rb_array)
{
    gfloat *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(gfloat, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = NUM2DBL(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_double(RBGIArguments *args,
                                                RBGIArgMetadata *metadata,
                                                VALUE rb_array)
{
    gdouble *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(gdouble, n_elements);
    for (i = 0; i < n_elements; i++) {
        raw_array[i] = NUM2DBL(RARRAY_AREF(rb_array, i));
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_gtype(RBGIArguments *args,
                                               RBGIArgMetadata *metadata,
                                               VALUE rb_array)
{
    GType *raw_array;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    raw_array = ALLOC_N(GType, n_elements);
    for (i = 0; i < n_elements; i++) {
        VALUE rb_type = RARRAY_AREF(rb_array, i);
        raw_array[i] = rbgobj_gtype_get(rb_type);
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_interface_struct(RBGIArguments *args,
                                                          RBGIArgMetadata *metadata,
                                                          VALUE rb_array)
{
    GIStructInfo *struct_info =
        (GIStructInfo *)(metadata->element_interface_info);
    guint8 *raw_array;
    gsize struct_size;
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    struct_size = g_struct_info_get_size(struct_info);
    raw_array = ALLOC_N(guint8, struct_size * n_elements);
    for (i = 0; i < n_elements; i++) {
        VALUE rb_element = RARRAY_AREF(rb_array, i);
        gpointer element;
        element = rb_gi_struct_info_from_ruby(struct_info, rb_element);
        memcpy(raw_array + (struct_size * i),
               element,
               struct_size);
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_interface_object(RBGIArguments *args,
                                                          RBGIArgMetadata *metadata,
                                                          VALUE rb_array)
{
    gpointer *raw_array;
    long i, n_elements, size;

    n_elements = RARRAY_LEN(rb_array);
    size = n_elements;
    if (metadata->zero_terminated_p) {
        size++;
    }
    raw_array = ALLOC_N(gpointer, size);
    for (i = 0; i < n_elements; i++) {
        VALUE rb_element = RARRAY_AREF(rb_array, i);
        raw_array[i] = RVAL2GOBJ(rb_element);
    }
    if (metadata->zero_terminated_p) {
        raw_array[i] = NULL;
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_interface(RBGIArguments *args,
                                                   RBGIArgMetadata *metadata,
                                                   VALUE rb_array)
{
    switch (metadata->element_interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: %s Ruby -> GIArgument(array/%s)[interface(%s)](%s)",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_info_type_to_string(metadata->element_interface_type),
                 g_type_name(metadata->element_interface_gtype));
        break;
      case GI_INFO_TYPE_STRUCT:
        rb_gi_arguments_in_init_arg_ruby_array_c_interface_struct(args,
                                                                  metadata,
                                                                  rb_array);
        break;
      case GI_INFO_TYPE_BOXED:
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: %s Ruby -> GIArgument(array/%s)[interface(%s)](%s)",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_info_type_to_string(metadata->element_interface_type),
                 g_type_name(metadata->element_interface_gtype));
        break;
      case GI_INFO_TYPE_OBJECT:
        rb_gi_arguments_in_init_arg_ruby_array_c_interface_object(args,
                                                                  metadata,
                                                                  rb_array);
        break;
      case GI_INFO_TYPE_INTERFACE:
      case GI_INFO_TYPE_CONSTANT:
      case GI_INFO_TYPE_INVALID_0:
      case GI_INFO_TYPE_UNION:
      case GI_INFO_TYPE_VALUE:
      case GI_INFO_TYPE_SIGNAL:
      case GI_INFO_TYPE_VFUNC:
      case GI_INFO_TYPE_PROPERTY:
      case GI_INFO_TYPE_FIELD:
      case GI_INFO_TYPE_ARG:
      case GI_INFO_TYPE_TYPE:
      case GI_INFO_TYPE_UNRESOLVED:
        rb_raise(rb_eNotImpError,
                 "TODO: %s Ruby -> GIArgument(array/%s)[interface(%s)](%s)",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_info_type_to_string(metadata->element_interface_type),
                 g_type_name(metadata->element_interface_gtype));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c(RBGIArguments *args,
                                         RBGIArgMetadata *metadata,
                                         VALUE rb_argument)
{
    GIArgument *array_argument;
    array_argument = &(g_array_index(args->in_args,
                                     GIArgument,
                                     metadata->in_arg_index));
    switch (metadata->element_type_tag) {
      case GI_TYPE_TAG_VOID:
        rb_raise(rb_eNotImpError,
                 "TODO: %s Ruby -> GIArgument(array/%s)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_TYPE_TAG_BOOLEAN:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_boolean(args,
                                                         metadata,
                                                         rb_argument);
        break;
      case GI_TYPE_TAG_INT8:
      case GI_TYPE_TAG_UINT8:
        if (RB_TYPE_P(rb_argument, RUBY_T_STRING)) {
            gchar *raw_array = RSTRING_PTR(rb_argument);
            gsize length = RSTRING_LEN(rb_argument);
            if (metadata->direction == GI_DIRECTION_INOUT) {
                array_argument->v_pointer = ALLOC(gpointer);
                *((gpointer *)(array_argument->v_pointer)) = raw_array;
            } else {
                array_argument->v_pointer = raw_array;
            }
            rb_gi_arguments_in_init_arg_ruby_array_set_length(args,
                                                              metadata,
                                                              length);
        } else {
            rb_argument = rbg_to_array(rb_argument);
            if (metadata->element_type_tag == GI_TYPE_TAG_INT8) {
                rb_gi_arguments_in_init_arg_ruby_array_c_int8(args,
                                                              metadata,
                                                              rb_argument);
            } else {
                rb_gi_arguments_in_init_arg_ruby_array_c_uint8(args,
                                                               metadata,
                                                               rb_argument);
            }
        }
        break;
      case GI_TYPE_TAG_INT16:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_int16(args,
                                                       metadata,
                                                       rb_argument);
        break;
      case GI_TYPE_TAG_UINT16:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_uint16(args,
                                                        metadata,
                                                        rb_argument);
        break;
      case GI_TYPE_TAG_INT32:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_int32(args,
                                                       metadata,
                                                       rb_argument);
        break;
      case GI_TYPE_TAG_UINT32:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_uint32(args,
                                                        metadata,
                                                        rb_argument);
        break;
      case GI_TYPE_TAG_INT64:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_int64(args,
                                                       metadata,
                                                       rb_argument);
        break;
      case GI_TYPE_TAG_UINT64:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_uint64(args,
                                                        metadata,
                                                        rb_argument);
        break;
      case GI_TYPE_TAG_FLOAT:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_float(args,
                                                       metadata,
                                                       rb_argument);
        break;
      case GI_TYPE_TAG_DOUBLE:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_double(args,
                                                        metadata,
                                                        rb_argument);
        break;
      case GI_TYPE_TAG_GTYPE:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_gtype(args,
                                                       metadata,
                                                       rb_argument);
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        {
            const gchar **raw_array;
            gsize length;
            rb_argument = rbg_to_array(rb_argument);
            raw_array = RVAL2STRV(rb_argument);
            length = RARRAY_LEN(rb_argument);
            if (metadata->direction == GI_DIRECTION_INOUT) {
                array_argument->v_pointer = ALLOC(gpointer);
                *((gpointer *)(array_argument->v_pointer)) = raw_array;
            } else {
                array_argument->v_pointer = raw_array;
            }
            rb_gi_arguments_in_init_arg_ruby_array_set_length(args,
                                                              metadata,
                                                              length);
        }
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: %s Ruby -> GIArgument(array/%s)[%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type_tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_argument = rbg_to_array(rb_argument);
        rb_gi_arguments_in_init_arg_ruby_array_c_interface(args,
                                                           metadata,
                                                           rb_argument);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: %s Ruby -> GIArgument(array/%s)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_array(RBGIArguments *args,
                                       RBGIArgMetadata *metadata)
{
    VALUE rb_argument = Qnil;

    if (RARRAY_LEN(args->rb_args) > metadata->rb_arg_index) {
        rb_argument = RARRAY_AREF(args->rb_args, metadata->rb_arg_index);
    }
    if (NIL_P(rb_argument) && metadata->may_be_null_p) {
        GIArgument *array_argument = &(g_array_index(args->in_args,
                                                     GIArgument,
                                                     metadata->in_arg_index));
        memset(array_argument, 0, sizeof(GIArgument));
        if (metadata->array_length_in_arg_index != -1) {
            GIArgument *length_argument =
                &(g_array_index(args->in_args,
                                GIArgument,
                                metadata->array_length_in_arg_index));
            memset(length_argument, 0, sizeof(GIArgument));
        }
        return;
    }

    switch (metadata->array_type) {
      case GI_ARRAY_TYPE_C:
        rb_gi_arguments_in_init_arg_ruby_array_c(args, metadata, rb_argument);
        break;
      case GI_ARRAY_TYPE_ARRAY:
      case GI_ARRAY_TYPE_PTR_ARRAY:
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: %s Ruby -> GIArgument(array/%s)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
    rb_gi_arguments_in_init_arg_ruby_transfer(args,
                                              metadata,
                                              rb_argument);
}

static void
rb_gi_arguments_in_init_arg_ruby_scalar_inout(RBGIArguments *args,
                                              RBGIArgMetadata *metadata,
                                              VALUE rb_argument)
{
    GIArgument *argument;
    GIArgument in_argument;

    argument = &(g_array_index(args->in_args,
                               GIArgument,
                               metadata->in_arg_index));
    rb_gi_value_argument_from_ruby(&in_argument,
                                   &(metadata->type_info),
                                   rb_argument,
                                   args->rb_receiver);

    switch (metadata->type_tag) {
    case GI_TYPE_TAG_VOID:
        break;
    case GI_TYPE_TAG_BOOLEAN:
        argument->v_pointer = ALLOC(gboolean);
        *((gboolean *)argument->v_pointer) = in_argument.v_boolean;
        break;
    case GI_TYPE_TAG_INT8:
        argument->v_pointer = ALLOC(gint8);
        *((gint8 *)argument->v_pointer) = in_argument.v_int8;
        break;
    case GI_TYPE_TAG_UINT8:
        argument->v_pointer = ALLOC(guint8);
        *((guint8 *)argument->v_pointer) = in_argument.v_uint8;
        break;
    case GI_TYPE_TAG_INT16:
        argument->v_pointer = ALLOC(gint16);
        *((gint16 *)argument->v_pointer) = in_argument.v_int16;
        break;
    case GI_TYPE_TAG_UINT16:
        argument->v_pointer = ALLOC(guint16);
        *((guint16 *)argument->v_pointer) = in_argument.v_uint16;
        break;
    case GI_TYPE_TAG_INT32:
        argument->v_pointer = ALLOC(gint32);
        *((gint32 *)argument->v_pointer) = in_argument.v_int32;
        break;
    case GI_TYPE_TAG_UINT32:
        argument->v_pointer = ALLOC(guint32);
        *((guint32 *)argument->v_pointer) = in_argument.v_uint32;
        break;
    case GI_TYPE_TAG_INT64:
        argument->v_pointer = ALLOC(gint64);
        *((gint64 *)argument->v_pointer) = in_argument.v_int64;
        break;
    case GI_TYPE_TAG_UINT64:
        argument->v_pointer = ALLOC(guint64);
        *((guint64 *)argument->v_pointer) = in_argument.v_uint64;
        break;
    case GI_TYPE_TAG_FLOAT:
        argument->v_pointer = ALLOC(gfloat);
        *((gfloat *)argument->v_pointer) = in_argument.v_float;
        break;
    case GI_TYPE_TAG_DOUBLE:
        argument->v_pointer = ALLOC(gdouble);
        *((gdouble *)argument->v_pointer) = in_argument.v_double;
        break;
    case GI_TYPE_TAG_GTYPE:
        argument->v_pointer = ALLOC(gsize);
        *((gsize *)argument->v_pointer) = in_argument.v_size;
        break;
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
        argument->v_pointer = ALLOC(gchar *);
        *((gchar **)argument->v_pointer) = in_argument.v_string;
        break;
    case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "should not be reached: Ruby -> GIArgument(%s)[%s]",
                 g_type_tag_to_string(metadata->type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
    case GI_TYPE_TAG_INTERFACE:
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
        argument->v_pointer = ALLOC(gpointer);
        *((gpointer *)argument->v_pointer) = in_argument.v_pointer;
        break;
    case GI_TYPE_TAG_ERROR:
        argument->v_pointer = ALLOC(GError *);
        *((GError **)argument->v_pointer) = in_argument.v_pointer;
        break;
    case GI_TYPE_TAG_UNICHAR:
        argument->v_pointer = ALLOC(gunichar);
        *((gunichar *)argument->v_pointer) = in_argument.v_uint32;
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_scalar(RBGIArguments *args,
                                        RBGIArgMetadata *metadata)
{
    GIArgument *argument;
    VALUE rb_argument = Qnil;

    argument = &(g_array_index(args->in_args,
                               GIArgument,
                               metadata->in_arg_index));
    if (RARRAY_LEN(args->rb_args) > metadata->rb_arg_index) {
        rb_argument = RARRAY_AREF(args->rb_args, metadata->rb_arg_index);
    }

    if (NIL_P(rb_argument)) {
        if (metadata->may_be_null_p) {
            memset(argument, 0, sizeof(GIArgument));
            return;
        } else {
            GIBaseInfo *klass;
            const char *suffix;

            klass = g_base_info_get_container(args->info);
            if (metadata->rb_arg_index == 1) {
                suffix = "st";
            } else if (metadata->rb_arg_index == 2) {
                suffix = "nd";
            } else if (metadata->rb_arg_index == 3) {
                suffix = "rd";
            } else {
                suffix = "th";
            }
            rb_raise(rb_eArgError,
                     "<%s%s%s%s%s>: the %u%s argument must not nil: <%s>",
                     g_base_info_get_namespace(args->info),
                     klass ? "::" : "",
                     klass ? g_base_info_get_name(klass) : "",
                     klass ? "#" : ".",
                     g_base_info_get_name(args->info),
                     metadata->rb_arg_index,
                     suffix,
                     metadata->name);
        }
    }

    if (metadata->direction == GI_DIRECTION_INOUT) {
        rb_gi_arguments_in_init_arg_ruby_scalar_inout(args,
                                                      metadata,
                                                      rb_argument);
    } else {
        rb_gi_value_argument_from_ruby(argument,
                                       &(metadata->type_info),
                                       rb_argument,
                                       args->rb_receiver);
        rb_gi_arguments_in_init_arg_ruby_transfer(args,
                                                  metadata,
                                                  rb_argument);
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
        rb_gi_arguments_in_init_arg_ruby_array(args, metadata);
    } else {
        rb_gi_arguments_in_init_arg_ruby_scalar(args, metadata);
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
        if (metadata->direction == GI_DIRECTION_OUT) {
            continue;
        }

        if (args->rb_mode_p) {
            rb_gi_arguments_in_init_arg_ruby(args, metadata);
        } else {
            rb_gi_arguments_in_init_arg_raw(args, metadata);
        }
    }
}

static void
rb_gi_arguments_in_clear_arg_array_c(RBGIArguments *args,
                                     RBGIArgMetadata *metadata,
                                     VALUE rb_argument)
{
    GIArgument *argument;
    gpointer raw_array;
    argument = &(g_array_index(args->in_args,
                               GIArgument,
                               metadata->in_arg_index));
    if (metadata->direction == GI_DIRECTION_INOUT) {
        raw_array = *((gpointer *)(argument->v_pointer));
    } else {
        raw_array = argument->v_pointer;
    }

    switch (metadata->element_type_tag) {
      case GI_TYPE_TAG_VOID:
        rb_raise(rb_eNotImpError,
                 "TODO: %s clear GIArgument(array/%s)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_TYPE_TAG_BOOLEAN:
        xfree(raw_array);
        break;
      case GI_TYPE_TAG_INT8:
      case GI_TYPE_TAG_UINT8:
        if (RB_TYPE_P(rb_argument, RUBY_T_STRING)) {
            /* Do nothing */
        } else {
            xfree(raw_array);
        }
        break;
      case GI_TYPE_TAG_INT16:
      case GI_TYPE_TAG_UINT16:
      case GI_TYPE_TAG_INT32:
      case GI_TYPE_TAG_UINT32:
      case GI_TYPE_TAG_INT64:
      case GI_TYPE_TAG_UINT64:
      case GI_TYPE_TAG_FLOAT:
      case GI_TYPE_TAG_DOUBLE:
      case GI_TYPE_TAG_GTYPE:
        xfree(raw_array);
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        g_free(raw_array);
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: %s clear GIArgument(array/%s)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_TYPE_TAG_INTERFACE:
        xfree(raw_array);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: %s clear GIArgument(array/%s)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_clear_arg_array(RBGIArguments *args,
                                   RBGIArgMetadata *metadata,
                                   VALUE rb_argument)
{
    switch (metadata->array_type) {
      case GI_ARRAY_TYPE_C:
        rb_gi_arguments_in_clear_arg_array_c(args, metadata, rb_argument);
        break;
      case GI_ARRAY_TYPE_ARRAY:
      case GI_ARRAY_TYPE_PTR_ARRAY:
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: %s clear GIArgument(array/%s)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_clear_arg_interface(RBGIArguments *args,
                                       RBGIArgMetadata *metadata,
                                       VALUE rb_argument)
{
    GIArgument *argument;
    gpointer value;
    argument = &(g_array_index(args->in_args,
                               GIArgument,
                               metadata->in_arg_index));
    if (metadata->direction == GI_DIRECTION_INOUT) {
        value = *((gpointer *)(argument->v_pointer));
    } else {
        value = argument->v_pointer;
    }

    switch (metadata->interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
        rb_raise(rb_eNotImpError,
                 "TODO: %s clear GIArgument(interface)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_INFO_TYPE_CALLBACK:
        if (metadata->callback_p) {
            /* Do nothing. */
            /* TODO: Should we check metadata->scope_type? */
        } else {
            rb_raise(rb_eNotImpError,
                     "TODO: %s clear GIArgument(interface)[%s][%s]",
                     rb_gi_direction_to_string(metadata->direction),
                     g_info_type_to_string(metadata->interface_type),
                     rb_gi_transfer_to_string(metadata->transfer));
        }
        break;
      case GI_INFO_TYPE_STRUCT:
        if (metadata->interface_gtype == G_TYPE_VALUE) {
            VALUE rb_cGLibValue = rb_const_get(mGLib, rb_intern("Value"));
            if (!RVAL2CBOOL(rb_obj_is_kind_of(rb_argument, rb_cGLibValue))) {
                GValue *gvalue = value;
                g_value_unset(gvalue);
                xfree(gvalue);
            }
        } else if (metadata->interface_gtype == G_TYPE_BYTES) {
            GBytes *bytes = value;
            g_bytes_unref(bytes);
        } else if (metadata->interface_gtype == G_TYPE_CLOSURE) {
            /* Do nothing. GRClousre is managed by referenced object. */
        } else {
            /*
            rb_raise(rb_eNotImpError,
                     "TODO: [%s] %s clear GIArgument(interface)[%s][%s][%s]",
                     metadata->name,
                     rb_gi_direction_to_string(metadata->direction),
                     g_info_type_to_string(metadata->interface_type),
                     g_type_name(metadata->interface_gtype),
                     rb_gi_transfer_to_string(metadata->transfer));
            */
        }
        break;
      case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s clear GIArgument(interface)[%s][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        break;
      case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: %s clear GIArgument(interface)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_INFO_TYPE_INVALID_0:
        g_assert_not_reached();
        break;
      case GI_INFO_TYPE_UNION:
        break;
      case GI_INFO_TYPE_VALUE:
      case GI_INFO_TYPE_SIGNAL:
      case GI_INFO_TYPE_VFUNC:
      case GI_INFO_TYPE_PROPERTY:
      case GI_INFO_TYPE_FIELD:
      case GI_INFO_TYPE_ARG:
      case GI_INFO_TYPE_TYPE:
      case GI_INFO_TYPE_UNRESOLVED:
        rb_raise(rb_eNotImpError,
                 "TODO: %s clear GIArgument(interface)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_clear_arg(RBGIArguments *args,
                             RBGIArgMetadata *metadata,
                             VALUE rb_argument)
{
    GIArgument *argument;
    argument = &(g_array_index(args->in_args,
                               GIArgument,
                               metadata->in_arg_index));

    switch (metadata->type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
      case GI_TYPE_TAG_UINT8:
      case GI_TYPE_TAG_INT16:
      case GI_TYPE_TAG_UINT16:
      case GI_TYPE_TAG_INT32:
      case GI_TYPE_TAG_UINT32:
      case GI_TYPE_TAG_INT64:
      case GI_TYPE_TAG_UINT64:
      case GI_TYPE_TAG_FLOAT:
      case GI_TYPE_TAG_DOUBLE:
      case GI_TYPE_TAG_GTYPE:
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_gi_arguments_in_clear_arg_array(args, metadata, rb_argument);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_in_clear_arg_interface(args, metadata, rb_argument);
        break;
      case GI_TYPE_TAG_GLIST:
        /* TODO */
        break;
      case GI_TYPE_TAG_GSLIST:
        /* TODO */
        break;
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s clear GIArgument(%s)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_TYPE_TAG_UNICHAR:
        if (metadata->direction == GI_DIRECTION_INOUT) {
            xfree(argument->v_pointer);
        }
        break;
      default:
        g_assert_not_reached();
        break;
    }

    if (metadata->direction == GI_DIRECTION_INOUT) {
        xfree(argument->v_pointer);
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
        VALUE rb_argument = Qnil;

        metadata = g_ptr_array_index(args->metadata, i);
        if (metadata->direction == GI_DIRECTION_OUT) {
            continue;
        }

        if (metadata->in_arg_index == -1) {
            continue;
        }

        if (RARRAY_LEN(args->rb_args) > metadata->rb_arg_index) {
            rb_argument = RARRAY_AREF(args->rb_args, metadata->rb_arg_index);
        }
        rb_gi_arguments_in_clear_arg(args, metadata, rb_argument);
    }
}
