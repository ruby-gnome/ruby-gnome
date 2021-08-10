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

static void
rb_gi_callback_data_destroy_notify(gpointer data)
{
    RBGICallbackData *callback_data = data;
    rb_gi_callback_data_free(callback_data);
}

VALUE
rb_gi_arguments_in_to_ruby(RBGIArguments *args)
{
    VALUE rb_in_args;
    guint i;

    rb_in_args = rb_ary_new_capa(args->metadata->len);
    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;
        GITypeInfo *type_info;
        VALUE rb_arg;

        metadata = g_ptr_array_index(args->metadata, i);

        if (metadata->direction == GI_DIRECTION_OUT) {
            continue;
        }
        if (metadata->closure_p) {
            continue;
        }

        type_info = g_arg_info_get_type(&(metadata->arg_info));
        /* TODO */
        rb_arg = GI_ARGUMENT2RVAL(metadata->in_arg,
                                  FALSE,
                                  type_info,
                                  args->in_args,
                                  args->out_args,
                                  args->metadata);
        rb_ary_push(rb_in_args, rb_arg);
    }

    return rb_in_args;
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

    /* TODO: metadata->free_func = ...; */
    arg_info = &(metadata->arg_info);
    callback_argument = metadata->in_arg;
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
        GITypeInfo *type_info = g_arg_info_get_type(arg_info);
        GICallbackInfo *callback_info = g_type_info_get_interface(type_info);
        callback = rb_gi_callback_new(callback_info, NULL);
        g_base_info_unref(callback_info);
        g_base_info_unref(type_info);
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
rb_gi_arguments_in_init_arg_ruby_void(RBGIArguments *args,
                                      RBGIArgMetadata *metadata)
{
    gpointer *target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = ALLOC(gpointer);
        metadata->in_arg->v_pointer = target;
    } else {
        target = &(metadata->in_arg->v_pointer);
    }

    if (!g_type_info_is_pointer(metadata->type.info)) {
        *target = NULL;
        return;
    }

    if (RB_TYPE_P(metadata->rb_arg, RUBY_T_STRING)) {
        *target = RSTRING_PTR(metadata->rb_arg);
    } else if (rbg_is_object(metadata->rb_arg)) {
        *target = RVAL2GOBJ(metadata->rb_arg);
    } else {
        *target = (gpointer)(NUM2ULL(metadata->rb_arg));
    }
}

static void
rb_gi_arguments_in_free_array_c_value(RBGIArguments *args,
                                      RBGIArgMetadata *metadata,
                                      gpointer user_data)
{
    gpointer target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = *((gpointer *)(metadata->in_arg->v_pointer));
        xfree(metadata->in_arg->v_pointer);
    } else {
        target = metadata->in_arg->v_pointer;
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        xfree(target);
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        break;
    }
}

static void
rb_gi_arguments_in_free_array_c_string(RBGIArguments *args,
                                       RBGIArgMetadata *metadata,
                                       gpointer user_data)
{
    if (metadata->direction == GI_DIRECTION_INOUT) {
        xfree(metadata->in_arg->v_pointer);
    }
}

static void
rb_gi_arguments_in_free_array_c_strv(RBGIArguments *args,
                                     RBGIArgMetadata *metadata,
                                     gpointer user_data)
{
    gpointer target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = *((gpointer *)(metadata->in_arg->v_pointer));
        xfree(metadata->in_arg->v_pointer);
    } else {
        target = metadata->in_arg->v_pointer;
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        g_free(target);
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        break;
    }
}

static void
rb_gi_arguments_in_free_array_c_filename(RBGIArguments *args,
                                         RBGIArgMetadata *metadata,
                                         gpointer user_data)
{
    gchar **target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = *((gchar ***)(metadata->in_arg->v_pointer));
        xfree(metadata->in_arg->v_pointer);
    } else {
        target = metadata->in_arg->v_pointer;
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        g_strfreev(target);
        break;
      case GI_TRANSFER_CONTAINER:
        {
            gsize i;
            for (i = 0; target[i]; i++) {
                g_free(target[i]);
            }
        }
        break;
      case GI_TRANSFER_EVERYTHING:
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_array_set_length(RBGIArguments *args,
                                                  RBGIArgMetadata *metadata,
                                                  gint64 length)
{
    GIArgument *length_argument = metadata->array_length_arg;
    if (!length_argument) {
        return;
    }

    RBGIArgMetadata *length_metadata = metadata->array_length_metadata;
    switch (length_metadata->type.tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid argument?: length[%s]",
                 g_type_tag_to_string(length_metadata->type.tag));
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
                 g_type_tag_to_string(length_metadata->type.tag));
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
        raw_array[i] = rbgobj_gtype_from_ruby(rb_type);
    }

    rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                     metadata,
                                                     rb_array,
                                                     raw_array);
}

static void
rb_gi_arguments_in_free_array_c_interface_struct(RBGIArguments *args,
                                                 RBGIArgMetadata *metadata,
                                                 gpointer user_data)
{
    gpointer target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = *((gpointer *)(metadata->in_arg->v_pointer));
        xfree(metadata->in_arg->v_pointer);
    } else {
        target = metadata->in_arg->v_pointer;
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        xfree(target);
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_interface_struct(RBGIArguments *args,
                                                          RBGIArgMetadata *metadata,
                                                          VALUE rb_array)
{
    GIStructInfo *struct_info =
        (GIStructInfo *)(metadata->element_type.interface_info);
    long i, n_elements;

    n_elements = RARRAY_LEN(rb_array);
    if (metadata->element_type.pointer_p) {
        if (metadata->transfer != GI_TRANSFER_NOTHING) {
            rb_raise(rb_eNotImpError,
                     "TODO: [%s::%s] %s "
                     "Ruby -> GIArgument(array/%s)[interface(%s)](%s)[%s]",
                     args->name,
                     metadata->name,
                     rb_gi_direction_to_string(metadata->direction),
                     rb_gi_array_type_to_string(metadata->array_type),
                     g_info_type_to_string(metadata->element_type.interface_type),
                     g_type_name(metadata->element_type.interface_gtype),
                     rb_gi_transfer_to_string(metadata->transfer));
        }
        gpointer *raw_array = ALLOC_N(gpointer, n_elements);
        for (i = 0; i < n_elements; i++) {
            VALUE rb_element = RARRAY_AREF(rb_array, i);
            gpointer element;
            element = rb_gi_struct_info_from_ruby(struct_info, rb_element);
            raw_array[i] = element;
        }
        rb_gi_arguments_in_init_arg_ruby_array_c_generic(args,
                                                         metadata,
                                                         rb_array,
                                                         raw_array);
    } else {
        gsize struct_size = g_struct_info_get_size(struct_info);
        guint8 *raw_array = ALLOC_N(guint8, struct_size * n_elements);
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

    metadata->free_func = rb_gi_arguments_in_free_array_c_interface_struct;
}

static void
rb_gi_arguments_in_free_array_c_interface_object(RBGIArguments *args,
                                                 RBGIArgMetadata *metadata,
                                                 gpointer user_data)
{
    gpointer target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = *((gpointer *)(metadata->in_arg->v_pointer));
        xfree(metadata->in_arg->v_pointer);
    } else {
        target = metadata->in_arg->v_pointer;
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        xfree(target);
        break;
      case GI_TRANSFER_CONTAINER:
        /* raw_array.each(&:unref)? */
        rb_raise(rb_eNotImpError,
                 "TODO: %s Ruby -> GIArgument(array/%s)[interface(%s)](%s)",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_info_type_to_string(metadata->element_type.interface_type),
                 g_type_name(metadata->element_type.interface_gtype));
        break;
      case GI_TRANSFER_EVERYTHING:
        break;
    }
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
    metadata->free_func = rb_gi_arguments_in_free_array_c_interface_object;
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c_interface(RBGIArguments *args,
                                                   RBGIArgMetadata *metadata,
                                                   VALUE rb_array)
{
    switch (metadata->element_type.interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: %s Ruby -> GIArgument(array/%s)[interface(%s)](%s)",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_info_type_to_string(metadata->element_type.interface_type),
                 g_type_name(metadata->element_type.interface_gtype));
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
                 g_info_type_to_string(metadata->element_type.interface_type),
                 g_type_name(metadata->element_type.interface_gtype));
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
                 g_info_type_to_string(metadata->element_type.interface_type),
                 g_type_name(metadata->element_type.interface_gtype));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_array_c(RBGIArguments *args,
                                         RBGIArgMetadata *metadata)
{
    VALUE rb_arg = metadata->rb_arg;

    metadata->free_func = rb_gi_arguments_in_free_array_c_value;
    switch (metadata->element_type.tag) {
      case GI_TYPE_TAG_VOID:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s Ruby -> GIArgument(array/%s)[%s][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_TYPE_TAG_BOOLEAN:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_boolean(args,
                                                         metadata,
                                                         rb_arg);
        break;
      case GI_TYPE_TAG_INT8:
      case GI_TYPE_TAG_UINT8:
        if (RB_TYPE_P(rb_arg, RUBY_T_STRING)) {
            gchar *raw_array = RSTRING_PTR(rb_arg);
            gsize length = RSTRING_LEN(rb_arg);
            if (metadata->transfer != GI_TRANSFER_NOTHING) {
                rb_raise(rb_eNotImpError,
                         "TODO: [%s] %s Ruby -> GIArgument(array/%s)[%s][%s]",
                         metadata->name,
                         rb_gi_direction_to_string(metadata->direction),
                         rb_gi_array_type_to_string(metadata->array_type),
                         g_type_tag_to_string(metadata->element_type.tag),
                         rb_gi_transfer_to_string(metadata->transfer));
            }
            if (metadata->direction == GI_DIRECTION_INOUT) {
                metadata->in_arg->v_pointer = ALLOC(gpointer);
                *((gpointer *)(metadata->in_arg->v_pointer)) = raw_array;
            } else {
                metadata->in_arg->v_pointer = raw_array;
            }
            rb_gi_arguments_in_init_arg_ruby_array_set_length(args,
                                                              metadata,
                                                              length);
            metadata->free_func = rb_gi_arguments_in_free_array_c_string;
        } else {
            rb_arg = rbg_to_array(rb_arg);
            if (metadata->element_type.tag == GI_TYPE_TAG_INT8) {
                rb_gi_arguments_in_init_arg_ruby_array_c_int8(args,
                                                              metadata,
                                                              rb_arg);
            } else {
                rb_gi_arguments_in_init_arg_ruby_array_c_uint8(args,
                                                               metadata,
                                                               rb_arg);
            }
        }
        break;
      case GI_TYPE_TAG_INT16:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_int16(args,
                                                       metadata,
                                                       rb_arg);
        break;
      case GI_TYPE_TAG_UINT16:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_uint16(args,
                                                        metadata,
                                                        rb_arg);
        break;
      case GI_TYPE_TAG_INT32:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_int32(args,
                                                       metadata,
                                                       rb_arg);
        break;
      case GI_TYPE_TAG_UINT32:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_uint32(args,
                                                        metadata,
                                                        rb_arg);
        break;
      case GI_TYPE_TAG_INT64:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_int64(args,
                                                       metadata,
                                                       rb_arg);
        break;
      case GI_TYPE_TAG_UINT64:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_uint64(args,
                                                        metadata,
                                                        rb_arg);
        break;
      case GI_TYPE_TAG_FLOAT:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_float(args,
                                                       metadata,
                                                       rb_arg);
        break;
      case GI_TYPE_TAG_DOUBLE:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_double(args,
                                                        metadata,
                                                        rb_arg);
        break;
      case GI_TYPE_TAG_GTYPE:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_gtype(args,
                                                       metadata,
                                                       rb_arg);
        break;
      case GI_TYPE_TAG_UTF8:
        /* Workaround for rsvg_handle_set_stylesheet():
           https://gitlab.gnome.org/GNOME/librsvg/-/issues/596 */
        if (strcmp(metadata->name, "css") == 0) {
            metadata->in_arg->v_pointer = (char *)RVAL2CSTR(rb_arg);
            rb_gi_arguments_in_init_arg_ruby_array_set_length(args,
                                                              metadata,
                                                              RSTRING_LEN(rb_arg));
            metadata->free_func = NULL;
        } else {
            GIArgument *array_argument = metadata->in_arg;
            gchar **raw_array;
            long length;
            rb_arg = rbg_to_array(rb_arg);
            if (metadata->transfer == GI_TRANSFER_EVERYTHING) {
                raw_array = rbg_rval2strv_dup(&rb_arg, &length);
            } else {
                raw_array = rbg_rval2strv(&rb_arg, &length);
            }
            if (metadata->direction == GI_DIRECTION_INOUT) {
                array_argument->v_pointer = ALLOC(gpointer);
                *((gpointer *)(array_argument->v_pointer)) = raw_array;
            } else {
                array_argument->v_pointer = raw_array;
            }
            rb_gi_arguments_in_init_arg_ruby_array_set_length(args,
                                                              metadata,
                                                              length);
            metadata->free_func = rb_gi_arguments_in_free_array_c_strv;
        }
        break;
      case GI_TYPE_TAG_FILENAME:
        {
            GIArgument *array_argument = metadata->in_arg;
            gchar **raw_array;
            long length;
            rb_arg = rbg_to_array(rb_arg);
            raw_array = rbg_rval2filenamev(&rb_arg, &length);
            if (metadata->direction == GI_DIRECTION_INOUT) {
                array_argument->v_pointer = ALLOC(gpointer);
                *((gpointer *)(array_argument->v_pointer)) = raw_array;
            } else {
                array_argument->v_pointer = raw_array;
            }
            rb_gi_arguments_in_init_arg_ruby_array_set_length(args,
                                                              metadata,
                                                              length);
            metadata->free_func = rb_gi_arguments_in_free_array_c_filename;
        }
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s::%s](%s) %s Ruby -> GIArgument(array/%s)[%s]",
                 args->namespace,
                 args->name,
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type.tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_arg = rbg_to_array(rb_arg);
        rb_gi_arguments_in_init_arg_ruby_array_c_interface(args,
                                                           metadata,
                                                           rb_arg);
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
                 g_type_tag_to_string(metadata->element_type.tag),
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
    if (NIL_P(metadata->rb_arg) && metadata->may_be_null_p) {
        memset(metadata->in_arg, 0, sizeof(GIArgument));
        if (metadata->array_length_arg) {
            memset(metadata->array_length_arg, 0, sizeof(GIArgument));
        }
        return;
    }

    switch (metadata->array_type) {
      case GI_ARRAY_TYPE_C:
        rb_gi_arguments_in_init_arg_ruby_array_c(args, metadata);
        break;
      case GI_ARRAY_TYPE_ARRAY:
      case GI_ARRAY_TYPE_PTR_ARRAY:
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: %s Ruby -> GIArgument(array/%s)[%s][%s]",
                 rb_gi_direction_to_string(metadata->direction),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_type_tag_to_string(metadata->element_type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_free_default(RBGIArguments *args,
                                RBGIArgMetadata *metadata,
                                gpointer user_data)
{
    if (metadata->direction == GI_DIRECTION_INOUT) {
        xfree(metadata->in_arg->v_pointer);
    }
}

static void
rb_gi_arguments_in_free_filename(RBGIArguments *args,
                                 RBGIArgMetadata *metadata,
                                 gpointer user_data)
{
    gchar *target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = *((gchar **)(metadata->in_arg->v_pointer));
        xfree(metadata->in_arg->v_pointer);
    } else {
        target = metadata->in_arg->v_string;
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        g_free(target);
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        break;
    }
}

static void
rb_gi_arguments_in_free_interface_struct(RBGIArguments *args,
                                         RBGIArgMetadata *metadata,
                                         gpointer user_data)
{
    if (metadata->direction == GI_DIRECTION_INOUT) {
        xfree(metadata->in_arg->v_pointer);
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        if (metadata->type.interface_gtype == G_TYPE_NONE) {
            rb_raise(rb_eNotImpError,
                     "TODO: [%s] free %s "
                     "Ruby -> GIArgument(interface)[%s][%s][%s]",
                     metadata->name,
                     rb_gi_direction_to_string(metadata->direction),
                     g_info_type_to_string(metadata->type.interface_type),
                     g_type_name(metadata->type.interface_gtype),
                     rb_gi_transfer_to_string(metadata->transfer));
        }
        rbgobj_boxed_unown(metadata->rb_arg);
        break;
    }
}

static void
rb_gi_arguments_in_free_interface_struct_gvalue(RBGIArguments *args,
                                                RBGIArgMetadata *metadata,
                                                gpointer user_data)
{
    GValue *target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = *((GValue **)(metadata->in_arg->v_pointer));
        xfree(metadata->in_arg->v_pointer);
    } else {
        target = metadata->in_arg->v_pointer;
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        g_value_unset(target);
        xfree(target);
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        break;
    }
}

static void
rb_gi_arguments_in_free_interface_struct_gbytes(RBGIArguments *args,
                                                RBGIArgMetadata *metadata,
                                                gpointer user_data)
{
    GBytes *target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = *((GBytes **)(metadata->in_arg->v_pointer));
        xfree(metadata->in_arg->v_pointer);
    } else {
        target = metadata->in_arg->v_pointer;
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        g_bytes_unref(target);
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        break;
    }
}

static void
rb_gi_arguments_in_free_interface_object(RBGIArguments *args,
                                         RBGIArgMetadata *metadata,
                                         gpointer user_data)
{
    if (metadata->direction == GI_DIRECTION_INOUT) {
        xfree(metadata->in_arg->v_pointer);
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_interface(RBGIArguments *args,
                                           RBGIArgMetadata *metadata)
{
    switch (metadata->type.interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s Ruby -> GIArgument(interface)[%s][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->type.interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_INFO_TYPE_STRUCT:
        metadata->free_func = rb_gi_arguments_in_free_interface_struct;
        if (metadata->type.interface_gtype == G_TYPE_VALUE) {
            GValue *gvalue;
            if (rbg_is_value(metadata->rb_arg)) {
                gvalue = RVAL2BOXED(metadata->rb_arg, G_TYPE_VALUE);
            } else {
                gvalue = ALLOC(GValue);
                memset(gvalue, 0, sizeof(GValue));
                rbgobj_initialize_gvalue(gvalue, metadata->rb_arg);
                metadata->free_func =
                    rb_gi_arguments_in_free_interface_struct_gvalue;
            }
            metadata->in_arg->v_pointer = gvalue;
        } else if (metadata->type.interface_gtype == G_TYPE_BYTES) {
            if (rbg_is_bytes(metadata->rb_arg)) {
                metadata->in_arg->v_pointer =
                    RVAL2BOXED(metadata->rb_arg, G_TYPE_BYTES);
            } else {
                VALUE rb_string;
                GBytes *gbytes;

                rb_string = StringValue(metadata->rb_arg);
                gbytes = g_bytes_new(RSTRING_PTR(rb_string),
                                     RSTRING_LEN(rb_string));
                metadata->in_arg->v_pointer = gbytes;
                metadata->free_func =
                    rb_gi_arguments_in_free_interface_struct_gbytes;
            }
        } else if (metadata->type.interface_gtype == G_TYPE_CLOSURE) {
            GClosure *rclosure = NULL;

            rclosure = g_rclosure_new(metadata->rb_arg, Qnil, NULL);
            g_rclosure_attach(rclosure, args->rb_receiver);
            metadata->in_arg->v_pointer = rclosure;
        } else {
            metadata->in_arg->v_pointer =
                rb_gi_struct_get_raw(metadata->rb_arg,
                                     metadata->type.interface_gtype);
        }
        break;
      case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s Ruby -> GIArgument(interface)[%s][%s][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->type.interface_type),
                 g_type_name(metadata->type.interface_gtype),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_INFO_TYPE_ENUM:
        if (metadata->type.interface_gtype == G_TYPE_NONE) {
            metadata->in_arg->v_int32 = NUM2INT(metadata->rb_arg);
        } else {
            metadata->in_arg->v_int32 =
                RVAL2GENUM(metadata->rb_arg, metadata->type.interface_gtype);
        }
        break;
      case GI_INFO_TYPE_FLAGS:
        if (metadata->type.interface_gtype == G_TYPE_NONE) {
            metadata->in_arg->v_int32 = NUM2INT(metadata->rb_arg);
        } else {
            metadata->in_arg->v_int32 =
                RVAL2GFLAGS(metadata->rb_arg, metadata->type.interface_gtype);
        }
        break;
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        metadata->in_arg->v_pointer = RVAL2GOBJ(metadata->rb_arg);
        if (metadata->in_arg->v_pointer &&
            metadata->type.interface_gtype != G_TYPE_NONE &&
            !G_TYPE_CHECK_INSTANCE_TYPE(metadata->in_arg->v_pointer,
                                        metadata->type.interface_gtype)) {
            rb_raise(rb_eArgError,
                     "must be <%s> object: <%" PRIsVALUE ">",
                     g_type_name(metadata->type.interface_gtype),
                     metadata->rb_arg);
        }
        if (metadata->in_arg->v_pointer &&
            metadata->transfer != GI_TRANSFER_NOTHING) {
            g_object_ref(metadata->in_arg->v_pointer);
        }
        metadata->free_func = rb_gi_arguments_in_free_interface_object;
        break;
      case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s][%s](%s) %s "
                 "Ruby -> GIArgument(interface)[%s][%s][%s]",
                 args->namespace,
                 args->name,
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->type.interface_type),
                 g_type_name(metadata->type.interface_gtype),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_INFO_TYPE_INVALID_0:
        g_assert_not_reached();
        break;
      case GI_INFO_TYPE_UNION:
        if (metadata->transfer != GI_TRANSFER_NOTHING) {
            rb_raise(rb_eNotImpError,
                     "TODO: [%s] %s Ruby -> GIArgument(interface)[%s][%s][%s]",
                     metadata->name,
                     rb_gi_direction_to_string(metadata->direction),
                     g_info_type_to_string(metadata->type.interface_type),
                     g_type_name(metadata->type.interface_gtype),
                     rb_gi_transfer_to_string(metadata->transfer));
        }
        if (metadata->type.interface_gtype == G_TYPE_NONE) {
            rb_raise(rb_eNotImpError,
                     "TODO: [%s] %s Ruby -> GIArgument(interface)[%s][%s][%s]",
                     metadata->name,
                     rb_gi_direction_to_string(metadata->direction),
                     g_info_type_to_string(metadata->type.interface_type),
                     g_type_name(metadata->type.interface_gtype),
                     rb_gi_transfer_to_string(metadata->transfer));
        } else {
            metadata->in_arg->v_pointer =
                RVAL2BOXED(metadata->rb_arg, metadata->type.interface_gtype);
        }
        break;
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
                 "TODO: [%s] %s Ruby -> GIArgument(interface)[%s][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->type.interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
    }

    if (metadata->direction == GI_DIRECTION_INOUT) {
        gpointer target = metadata->in_arg->v_pointer;
        metadata->in_arg->v_pointer = ALLOC(gpointer);
        *((gpointer *)(metadata->in_arg->v_pointer)) = target;
    }
}

static void
rb_gi_arguments_in_free_list(RBGIArguments *args,
                             RBGIArgMetadata *metadata,
                             gpointer user_data)
{
    gpointer target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = *((gpointer *)(metadata->in_arg->v_pointer));
        xfree(metadata->in_arg->v_pointer);
    } else {
        target = metadata->in_arg->v_pointer;
    }

    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        if (metadata->type.tag == GI_TYPE_TAG_GLIST) {
            g_list_free(target);
        } else {
            g_slist_free(target);
        }
        break;
      case GI_TRANSFER_CONTAINER:
        break;
      default:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] free %s "
                 "Ruby -> GIArgument(%s)[%s][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 g_type_tag_to_string(metadata->element_type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_ruby_list(RBGIArguments *args,
                                      RBGIArgMetadata *metadata)
{
    GType gtype = metadata->element_type.interface_gtype;

    metadata->free_func = rb_gi_arguments_in_free_list;
    switch (metadata->element_type.tag) {
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
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s Ruby -> GIArgument(%s)[%s][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 g_type_tag_to_string(metadata->element_type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_TYPE_TAG_INTERFACE:
        if (metadata->type.tag == GI_TYPE_TAG_GLIST) {
            metadata->in_arg->v_pointer = RVAL2GOBJGLIST(metadata->rb_arg);
        } else {
            metadata->in_arg->v_pointer = RVAL2GOBJGSLIST(metadata->rb_arg);
        }
        if (gtype != G_TYPE_NONE) {
            if (metadata->type.tag == GI_TYPE_TAG_GLIST) {
                GList *node;
                for (node = metadata->in_arg->v_pointer;
                     node;
                     node = g_list_next(node)) {
                    GObject *object = node->data;
                    if (!G_TYPE_CHECK_INSTANCE_TYPE(object, gtype)) {
                        g_list_free(metadata->in_arg->v_pointer);
                        rb_raise(rb_eArgError,
                                 "must be <%s> objects: <%" PRIsVALUE ">",
                                 g_type_name(gtype),
                                 metadata->rb_arg);
                    }
                }
            } else {
                GSList *node;
                for (node = metadata->in_arg->v_pointer;
                     node;
                     node = g_slist_next(node)) {
                    GObject *object = node->data;
                    if (!G_TYPE_CHECK_INSTANCE_TYPE(object, gtype)) {
                        g_slist_free(metadata->in_arg->v_pointer);
                        rb_raise(rb_eArgError,
                                 "must be <%s> objects: <%" PRIsVALUE ">",
                                 g_type_name(gtype),
                                 metadata->rb_arg);
                    }
                }
            }
        }
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s Ruby -> GIArgument(%s)[%s][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 g_type_tag_to_string(metadata->element_type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    if (metadata->direction == GI_DIRECTION_INOUT) {
        gpointer target = metadata->in_arg->v_pointer;
        metadata->in_arg->v_pointer = ALLOC(gpointer);
        *((gpointer *)(metadata->in_arg->v_pointer)) = target;
    }
}

typedef struct {
    VALUE rb_value;
    RBGIArgMetadataType *type;
    const gchar *context;
} RubyToCData;

typedef gpointer (*RubyToCFunc)(RubyToCData *data);

static gpointer
ruby_to_c_utf8(RubyToCData *data)
{
    return g_strdup(RVAL2CSTR(data->rb_value));
}

static gpointer
ruby_to_c_interface_enum(RubyToCData *data)
{
    gint32 value;
    if (data->type->interface_gtype == G_TYPE_NONE) {
        value = NUM2INT(data->rb_value);
    } else {
        value = RVAL2GENUM(data->rb_value, data->type->interface_gtype);
    }
    return GINT_TO_POINTER(value);
}

typedef struct {
    GIArgument *argument;
    GITypeInfo *type_info;
    VALUE rb_argument;
    VALUE self;
} ArgumentFromRubyData;

typedef struct {
    GHashTable *hash_table;
    RubyToCFunc key_ruby_to_c_func;
    RubyToCData *key_ruby_to_c_data;
    RubyToCFunc value_ruby_to_c_func;
    RubyToCData *value_ruby_to_c_data;
} ArgumentFromRubyGHashData;

static int
rb_gi_arguments_in_init_arg_ruby_ghash_convert(VALUE rb_key,
                                               VALUE rb_value,
                                               VALUE user_data)
{
    ArgumentFromRubyGHashData *data = (ArgumentFromRubyGHashData *)user_data;
    gpointer key;
    gpointer value;
    data->key_ruby_to_c_data->rb_value = rb_key;
    key = data->key_ruby_to_c_func(data->key_ruby_to_c_data);
    data->value_ruby_to_c_data->rb_value = rb_value;
    value = data->key_ruby_to_c_func(data->value_ruby_to_c_data);
    g_hash_table_insert(data->hash_table, key, value);
    return ST_CONTINUE;
}

static VALUE
rb_gi_arguments_in_init_arg_ruby_ghash_body(VALUE value_data)
{
    RBGIArgMetadata *metadata = (RBGIArgMetadata *)value_data;
    GHashFunc hash_func = NULL;
    GEqualFunc equal_func = NULL;
    GDestroyNotify key_destroy_func = NULL;
    GDestroyNotify value_destroy_func = NULL;
    RubyToCFunc key_ruby_to_c_func = NULL;
    RubyToCData key_ruby_to_c_data;
    RubyToCFunc value_ruby_to_c_func = NULL;
    RubyToCData value_ruby_to_c_data;
    ArgumentFromRubyGHashData convert_data;

    key_ruby_to_c_data.type = &(metadata->key_type);
    switch (metadata->key_type.tag) {
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
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(GHash)[key][%s]",
                 g_type_tag_to_string(metadata->key_type.tag));
        break;
      case GI_TYPE_TAG_UTF8:
        hash_func = g_str_hash;
        equal_func = g_str_equal;
        key_destroy_func = g_free;
        key_ruby_to_c_func = ruby_to_c_utf8;
        key_ruby_to_c_data.context = "Ruby -> GIArgument(GHash)[key][utf8]";
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
      case GI_TYPE_TAG_INTERFACE:
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(GHash)[key][%s]",
                 g_type_tag_to_string(metadata->key_type.tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    value_ruby_to_c_data.type = &(metadata->value_type);
    switch (metadata->value_type.tag) {
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
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(GHash)[value][%s]",
                 g_type_tag_to_string(metadata->value_type.tag));
        break;
      case GI_TYPE_TAG_UTF8:
        value_destroy_func = g_free;
        value_ruby_to_c_func = ruby_to_c_utf8;
        value_ruby_to_c_data.context = "Ruby -> GIArgument(GHash)[value][utf8]";
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(GHash)[value][%s]",
                 g_type_tag_to_string(metadata->value_type.tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        switch (metadata->value_type.interface_type) {
          case GI_INFO_TYPE_INVALID:
          case GI_INFO_TYPE_FUNCTION:
          case GI_INFO_TYPE_CALLBACK:
          case GI_INFO_TYPE_STRUCT:
          case GI_INFO_TYPE_BOXED:
            rb_raise(rb_eNotImpError,
                     "TODO: Ruby -> GIArgument(GHash)[value][%s][%s]",
                     g_type_tag_to_string(metadata->value_type.tag),
                     g_info_type_to_string(metadata->value_type.interface_type));
            break;
          case GI_INFO_TYPE_ENUM:
            value_destroy_func = NULL;
            value_ruby_to_c_func = ruby_to_c_interface_enum;
            value_ruby_to_c_data.context =
                "Ruby -> GIArgument(GHash)[value][interface]";
            break;
          case GI_INFO_TYPE_FLAGS:
          case GI_INFO_TYPE_OBJECT:
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
                     "TODO: Ruby -> GIArgument(GHash)[value][%s][%s]",
                     g_type_tag_to_string(metadata->value_type.tag),
                     g_info_type_to_string(metadata->value_type.interface_type));
            break;
          default:
            g_assert_not_reached();
            break;
        }
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(GHash)[value][%s]",
                 g_type_tag_to_string(metadata->value_type.tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    metadata->in_arg->v_pointer = g_hash_table_new_full(hash_func,
                                                        equal_func,
                                                        key_destroy_func,
                                                        value_destroy_func);
    convert_data.hash_table = metadata->in_arg->v_pointer;
    convert_data.key_ruby_to_c_func = key_ruby_to_c_func;
    convert_data.key_ruby_to_c_data = &key_ruby_to_c_data;
    convert_data.value_ruby_to_c_func = value_ruby_to_c_func;
    convert_data.value_ruby_to_c_data = &value_ruby_to_c_data;
    rb_hash_foreach(metadata->rb_arg,
                    rb_gi_arguments_in_init_arg_ruby_ghash_convert,
                    (VALUE)&convert_data);

    return Qnil;
}

static VALUE
rb_gi_arguments_in_init_arg_ruby_ghash_rescue(VALUE value_data,
                                              VALUE exception)
{
    RBGIArgMetadata *metadata = (RBGIArgMetadata *)value_data;

    if (metadata->in_arg->v_pointer) {
        g_hash_table_unref(metadata->in_arg->v_pointer);
        metadata->in_arg->v_pointer = NULL;
    }

    rb_exc_raise(exception);

    return Qnil;
}

static void
rb_gi_arguments_in_free_ghash(RBGIArguments *args,
                              RBGIArgMetadata *metadata,
                              gpointer user_data)
{
    GHashTable *target;
    if (metadata->direction == GI_DIRECTION_INOUT) {
        target = *((GHashTable **)(metadata->in_arg->v_pointer));
        xfree(metadata->in_arg->v_pointer);
    } else {
        target = metadata->in_arg->v_pointer;
    }
    g_hash_table_unref(target);
}

static void
rb_gi_arguments_in_init_arg_ruby_ghash(RBGIArguments *args,
                                       RBGIArgMetadata *metadata)
{
    if (metadata->transfer != GI_TRANSFER_NOTHING) {
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] free %s "
                 "Ruby -> GIArgument(%s){%s,%s}[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 g_type_tag_to_string(metadata->key_type.tag),
                 g_type_tag_to_string(metadata->value_type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
    }
    metadata->in_arg->v_pointer = NULL;
    rb_rescue(rb_gi_arguments_in_init_arg_ruby_ghash_body, (VALUE)metadata,
              rb_gi_arguments_in_init_arg_ruby_ghash_rescue, (VALUE)metadata);
    if (metadata->direction == GI_DIRECTION_INOUT) {
        GHashTable *target = metadata->in_arg->v_pointer;
        metadata->in_arg->v_pointer = ALLOC(GHashTable *);
        *((GHashTable **)(metadata->in_arg->v_pointer)) = target;
    }
    metadata->free_func = rb_gi_arguments_in_free_ghash;
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
        return;
    }

    if (NIL_P(metadata->rb_arg)) {
        GIBaseInfo *klass;
        const char *suffix;

        if (metadata->may_be_null_p) {
            memset(metadata->in_arg, 0, sizeof(GIArgument));
            return;
        }

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

    metadata->free_func = rb_gi_arguments_in_free_default;
    switch (metadata->type.tag) {
      case GI_TYPE_TAG_VOID:
        rb_gi_arguments_in_init_arg_ruby_void(args, metadata);
        break;
      case GI_TYPE_TAG_BOOLEAN:
        {
            gboolean *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(gboolean);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_boolean);
            }
            *target = RVAL2CBOOL(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_INT8:
        {
            gint8 *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(gint8);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_int8);
            }
            *target = NUM2CHR(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_UINT8:
        {
            guint8 *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(guint8);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_uint8);
            }
            *target = (guint8)NUM2CHR(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_INT16:
        {
            gint16 *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(gint16);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_int16);
            }
            *target = NUM2SHORT(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_UINT16:
        {
            guint16 *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(guint16);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_uint16);
            }
            *target = NUM2USHORT(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_INT32:
        {
            gint32 *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(gint32);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_int32);
            }
            *target = NUM2INT(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_UINT32:
        {
            guint32 *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(guint32);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_uint32);
            }
            *target = NUM2UINT(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_INT64:
        {
            gint64 *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(gint64);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_int64);
            }
            *target = NUM2LL(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_UINT64:
        {
            guint64 *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(guint64);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_uint64);
            }
            *target = NUM2ULL(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_FLOAT:
        {
            gfloat *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(gfloat);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_float);
            }
            *target = NUM2DBL(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_DOUBLE:
        {
            gdouble *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(gdouble);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_double);
            }
            *target = NUM2DBL(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_GTYPE:
        {
            GType *target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(GType);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_size);
            }
            *target = rbgobj_gtype_from_ruby(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_UTF8:
        {
            gchar **target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(gchar *);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_string);
            }
            *target = (gchar *)RVAL2CSTR_ACCEPT_SYMBOL(metadata->rb_arg);
        }
        break;
      case GI_TYPE_TAG_FILENAME:
        {
            gchar **target;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(gchar *);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_string);
            }
            *target = rbg_filename_from_ruby(metadata->rb_arg);
            metadata->free_func = rb_gi_arguments_in_free_filename;
        }
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "should not be reached: Ruby -> GIArgument(%s)",
                 g_type_tag_to_string(metadata->type.tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_in_init_arg_ruby_interface(args, metadata);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
        rb_gi_arguments_in_init_arg_ruby_list(args, metadata);
        break;
      case GI_TYPE_TAG_GHASH:
        rb_gi_arguments_in_init_arg_ruby_ghash(args, metadata);
        break;
      case GI_TYPE_TAG_ERROR:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s Ruby -> GIArgument(%s)",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag));
        break;
      case GI_TYPE_TAG_UNICHAR:
        {
            gunichar *target;
            VALUE rb_unichar;
            if (metadata->direction == GI_DIRECTION_INOUT) {
                target = ALLOC(gunichar);
                metadata->in_arg->v_pointer = target;
            } else {
                target = &(metadata->in_arg->v_uint32);
            }
            if (RB_TYPE_P(metadata->rb_arg, RUBY_T_STRING)) {
                VALUE rb_codepoints;
                if (rb_str_strlen(metadata->rb_arg) != 1) {
                    rb_raise(rb_eArgError,
                             "[%s][%s] must be one character: %+" PRIsVALUE,
                             metadata->name,
                             g_type_tag_to_string(metadata->type.tag),
                             metadata->rb_arg);
                }
                rb_codepoints = rb_funcall(metadata->rb_arg,
                                           rb_intern("codepoints"),
                                           0);
                rb_unichar = RARRAY_PTR(rb_codepoints)[0];
            } else {
                rb_unichar = metadata->rb_arg;
            }
            *target = NUM2UINT(rb_unichar);
        }
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_raw_interface(RBGIArguments *args,
                                          RBGIArgMetadata *metadata)
{
    switch (metadata->type.interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw argument -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(metadata->type.interface_type),
                 g_base_info_get_name(metadata->type.interface_info));
        break;
    case GI_INFO_TYPE_STRUCT:
      metadata->in_arg->v_pointer = *((gpointer *)(args->raw_args[metadata->index]));
      break;
    case GI_INFO_TYPE_BOXED:
    case GI_INFO_TYPE_ENUM:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw argument -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(metadata->type.interface_type),
                 g_base_info_get_name(metadata->type.interface_info));
        break;
    case GI_INFO_TYPE_FLAGS:
      metadata->in_arg->v_int32= *((gint32 *)(args->raw_args[metadata->index]));
      break;
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
      metadata->in_arg->v_pointer = *((gpointer *)(args->raw_args[metadata->index]));
      break;
    case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw argument -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(metadata->type.interface_type),
                 g_base_info_get_name(metadata->type.interface_info));
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
                 g_info_type_to_string(metadata->type.interface_type),
                 g_base_info_get_name(metadata->type.interface_info));
        break;
    }
}

static void
rb_gi_arguments_in_init_arg_raw(RBGIArguments *args,
                                RBGIArgMetadata *metadata)
{
    switch (metadata->type.tag) {
      case GI_TYPE_TAG_VOID:
        metadata->in_arg->v_pointer = *((gpointer *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_BOOLEAN:
        metadata->in_arg->v_boolean = *((gboolean *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_INT8:
        metadata->in_arg->v_int8 = *((gint8 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UINT8:
        metadata->in_arg->v_uint8 = *((guint8 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_INT16:
        metadata->in_arg->v_int16 = *((gint16 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UINT16:
        metadata->in_arg->v_uint16 = *((guint16 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_INT32:
        metadata->in_arg->v_int32 = *((gint32 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UINT32:
        metadata->in_arg->v_uint32 = *((guint32 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_INT64:
        metadata->in_arg->v_int64 = *((gint64 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UINT64:
        metadata->in_arg->v_uint64 = *((guint64 *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_FLOAT:
        metadata->in_arg->v_float = *((gfloat *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_DOUBLE:
        metadata->in_arg->v_double = *((gdouble *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_GTYPE:
        metadata->in_arg->v_size = *((gsize *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        metadata->in_arg->v_string = *((gchar **)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_ARRAY:
        metadata->in_arg->v_pointer = *((gpointer *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_in_init_arg_raw_interface(args, metadata);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        metadata->in_arg->v_pointer = *((gpointer *)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_ERROR:
        metadata->in_arg->v_pointer = *((GError **)(args->raw_args[metadata->index]));
        break;
      case GI_TYPE_TAG_UNICHAR:
        metadata->in_arg->v_uint32 = *((gunichar *)(args->raw_args[metadata->index]));
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

void
rb_gi_arguments_in_clear(RBGIArguments *args)
{
    guint i;

    if (!args->rb_mode_p) {
        return;
    }

    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;

        metadata = g_ptr_array_index(args->metadata, i);
        if (metadata->direction == GI_DIRECTION_OUT) {
            continue;
        }

        if (metadata->in_arg_index == -1) {
            continue;
        }

        if (metadata->free_func) {
            metadata->free_func(args, metadata, metadata->free_func_data);
        }
    }
}
