/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2024  Ruby-GNOME Project Team
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

static gboolean
rb_gi_is_registered_type(GIInfoType type)
{
    switch (type) {
      case GI_INFO_TYPE_STRUCT:
      case GI_INFO_TYPE_BOXED:
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
      case GI_INFO_TYPE_UNION:
        return TRUE;
      default:
        return FALSE;
    }
}

static void
rb_gi_arg_metadata_type_init(RBGIArgMetadataType *type,
                             GITypeInfo *type_info)
{
    type->info = type_info;
    type->pointer_p = FALSE;
    type->tag = GI_TYPE_TAG_VOID;
    type->interface_info = NULL;
    type->interface_type = GI_INFO_TYPE_INVALID;
    type->interface_gtype = G_TYPE_INVALID;

    if (type->info) {
        type->pointer_p = g_type_info_is_pointer(type->info);
        type->tag = g_type_info_get_tag(type->info);
    }
    if (type->tag == GI_TYPE_TAG_INTERFACE) {
        type->interface_info = g_type_info_get_interface(type_info);
        type->interface_type = g_base_info_get_type(type->interface_info);
        if (rb_gi_is_registered_type(type->interface_type)) {
            type->interface_gtype =
                g_registered_type_info_get_g_type(type->interface_info);
        }
    }
}

static void
rb_gi_arg_metadata_type_clear(RBGIArgMetadataType *type)
{
    if (type->interface_info) {
        g_base_info_unref(type->interface_info);
    }
    if (type->info) {
        g_base_info_unref(type->info);
    }
}

void
rb_gi_arg_metadata_init_type_info(RBGIArgMetadata *metadata,
                                  GITypeInfo *type_info)
{
    metadata->type_info = type_info;
    rb_gi_arg_metadata_type_init(&(metadata->type), type_info);
    rb_gi_arg_metadata_type_init(&(metadata->element_type), NULL);
    rb_gi_arg_metadata_type_init(&(metadata->key_type), NULL);
    rb_gi_arg_metadata_type_init(&(metadata->value_type), NULL);
    metadata->scope_type = GI_SCOPE_TYPE_INVALID;
    metadata->direction = GI_DIRECTION_IN;
    metadata->transfer = GI_TRANSFER_NOTHING;
    metadata->array_type = GI_ARRAY_TYPE_C;
    metadata->callback_p = FALSE;
    metadata->closure_p = FALSE;
    metadata->destroy_p = FALSE;
    metadata->interface_p = (metadata->type.tag == GI_TYPE_TAG_INTERFACE);
    metadata->array_p = (metadata->type.tag == GI_TYPE_TAG_ARRAY);
    metadata->array_length_p = FALSE;
    metadata->may_be_null_p = FALSE;
    metadata->caller_allocates_p = FALSE;
    metadata->zero_terminated_p = FALSE;
    metadata->input_buffer_p = FALSE;
    metadata->output_buffer_p = FALSE;
    metadata->in_arg_index = -1;
    metadata->closure_in_arg_index = -1;
    metadata->destroy_in_arg_index = -1;
    metadata->rb_arg_index = -1;
    metadata->out_arg_index = -1;
    metadata->in_arg = NULL;
    metadata->out_arg = NULL;
    metadata->rb_arg = Qnil;
    metadata->array_metadata = NULL;
    metadata->array_length_metadata = NULL;
    metadata->array_length_arg = NULL;
    metadata->free_func = NULL;
    metadata->free_func_data = NULL;

    switch (metadata->type.tag) {
      case GI_TYPE_TAG_ARRAY:
        metadata->zero_terminated_p =
            g_type_info_is_zero_terminated(type_info);
        metadata->array_type = g_type_info_get_array_type(type_info);
        rb_gi_arg_metadata_type_init(&(metadata->element_type),
                                     g_type_info_get_param_type(type_info, 0));
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
        rb_gi_arg_metadata_type_init(&(metadata->element_type),
                                     g_type_info_get_param_type(type_info, 0));
        break;
      case GI_TYPE_TAG_GHASH:
        rb_gi_arg_metadata_type_init(&(metadata->key_type),
                                     g_type_info_get_param_type(type_info, 0));
        rb_gi_arg_metadata_type_init(&(metadata->value_type),
                                     g_type_info_get_param_type(type_info, 1));
        break;
      default:
        break;
    }
}

void
rb_gi_arg_metadata_init_struct_info(RBGIArgMetadata *metadata,
                                    GIStructInfo *struct_info,
                                    gpointer struct_memory)
{
    metadata->struct_info = struct_info;
    metadata->struct_memory = struct_memory;
}

static RBGIArgMetadata *
rb_gi_arg_metadata_new(GICallableInfo *callable_info, gint i)
{
    /* We can't use ALLOC() (Ruby API) here because this may be called
       in a thread that isn't the Ruby's main thread. */
    RBGIArgMetadata *metadata = g_new(RBGIArgMetadata, 1);
    metadata->callable_info = callable_info;
    GIArgInfo *arg_info = &(metadata->arg_info);
    g_callable_info_load_arg(callable_info, i, arg_info);
    metadata->name = g_base_info_get_name(arg_info);
    metadata->struct_info = NULL;
    metadata->struct_memory = NULL;
    metadata->index = i;

    rb_gi_arg_metadata_init_type_info(metadata,
                                      g_arg_info_get_type(arg_info));

    metadata->scope_type = g_arg_info_get_scope(arg_info);
    metadata->direction = g_arg_info_get_direction(arg_info);
    metadata->transfer = g_arg_info_get_ownership_transfer(arg_info);
    metadata->callback_p = (metadata->scope_type != GI_SCOPE_TYPE_INVALID);
    metadata->may_be_null_p = g_arg_info_may_be_null(arg_info);
    metadata->caller_allocates_p = g_arg_info_is_caller_allocates(arg_info);
    metadata->input_buffer_p = rb_gi_arg_info_is_input_buffer(arg_info);
    metadata->output_buffer_p = rb_gi_arg_info_is_output_buffer(arg_info);

    return metadata;
}

void
rb_gi_arg_metadata_clear(RBGIArgMetadata *metadata)
{
    rb_gi_arg_metadata_type_clear(&(metadata->value_type));
    rb_gi_arg_metadata_type_clear(&(metadata->key_type));
    rb_gi_arg_metadata_type_clear(&(metadata->element_type));
    rb_gi_arg_metadata_type_clear(&(metadata->type));
}

void
rb_gi_arg_metadata_free(RBGIArgMetadata *metadata)
{
    rb_gi_arg_metadata_clear(metadata);
    g_free(metadata);
}

static void
rb_gi_arguments_allocate(RBGIArguments *args)
{
    if (!args->info) {
        return;
    }

    gint n_args = g_callable_info_get_n_args(args->info);
    gint i;
    for (i = 0; i < n_args; i++) {
        GIArgument argument = {0};
        RBGIArgMetadata *metadata;
        GIDirection direction;

        metadata = rb_gi_arg_metadata_new(args->info, i);

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

    for (i = 0; i < n_args; i++) {
        RBGIArgMetadata *metadata;
        GIDirection direction;

        metadata = g_ptr_array_index(args->metadata, i);
        direction = metadata->direction;
        if (direction == GI_DIRECTION_IN || direction == GI_DIRECTION_INOUT) {
            metadata->in_arg = &g_array_index(args->in_args,
                                              GIArgument,
                                              metadata->in_arg_index);
        }
        if (direction == GI_DIRECTION_OUT || direction == GI_DIRECTION_INOUT) {
            metadata->out_arg = &g_array_index(args->out_args,
                                               GIArgument,
                                               metadata->out_arg_index);
        }
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
        GITypeInfo *type_info;
        gint array_length_index = -1;

        array_metadata = g_ptr_array_index(metadata, i);
        type_info = array_metadata->type.info;
        if (!array_metadata->array_p) {
            continue;
        }

        array_length_index = g_type_info_get_array_length(type_info);
        if (array_length_index == -1) {
            continue;
        }

        array_length_metadata = g_ptr_array_index(metadata, array_length_index);
        array_length_metadata->array_length_p = TRUE;
        array_length_metadata->rb_arg_index = -1;
        array_length_metadata->array_metadata = array_metadata;
        array_metadata->array_length_metadata = array_length_metadata;
        if (array_length_metadata->in_arg) {
            array_metadata->array_length_arg = array_length_metadata->in_arg;
        } else {
            array_metadata->array_length_arg = array_length_metadata->out_arg;
        }
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

        if (metadata->in_arg_index == -1 &&
            !metadata->output_buffer_p) {
            continue;
        }

        metadata->rb_arg_index = rb_arg_index;
        if (RARRAY_LEN(args->rb_args) > metadata->rb_arg_index) {
            metadata->rb_arg =
                RARRAY_AREF(args->rb_args, metadata->rb_arg_index);
        }
        rb_arg_index++;
    }
}

static void
rb_gi_arguments_fill_metadata(RBGIArguments *args)
{
    if (!args->metadata) {
        return;
    }

    rb_gi_arguments_fill_metadata_callback(args);
    rb_gi_arguments_fill_metadata_array(args);
    rb_gi_arguments_fill_metadata_array_from_callable_info(args);
    if (args->rb_mode_p) {
        rb_gi_arguments_fill_metadata_rb_arg_index(args);
    }
}

static void
rb_gi_arguments_fill_rb_args(RBGIArguments *args)
{
    rb_gi_arguments_in_init(args);
    rb_gi_arguments_out_init(args);
}

static void
rb_gi_arguments_fill_raw_args(RBGIArguments *args)
{
    guint i;

    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;

        metadata = g_ptr_array_index(args->metadata, i);
        if (metadata->direction == GI_DIRECTION_INOUT) {
            GIArgument *argument = &g_array_index(args->in_args,
                                                  GIArgument,
                                                  metadata->in_arg_index);
            argument->v_pointer = *((gpointer *)(args->raw_args[i]));
        }
    }

    rb_gi_arguments_in_init(args);
    rb_gi_arguments_out_init(args);
}

static void
rb_gi_arguments_metadata_free(gpointer data)
{
    RBGIArgMetadata *metadata = data;
    if (metadata->scope_type == GI_SCOPE_TYPE_ASYNC) {
        /* We can't free async scope argument's metadata immediately
         * because async scope argument is called later and it uses its
         * metadata. Its metadata is freed when async scope argument is called.
         *
         * Exception: We can free GDestroyNotify's metadata
         * immediately because GDestroyNotify is only used in
         * Ruby/GObjectIntrospection and Ruby/GObjectIntorspection
         * doesn't use GDestroyNotify's metadata. */
        if (!metadata->destroy_p) {
            return;
        }
    }
    /* We can't free notified scope argument's metadata immediately
     * because notified scope argument is called later and it uses its
     * metadata. Its metadata is freed when its GDestroyNotify is called. */
    if (metadata->scope_type == GI_SCOPE_TYPE_NOTIFIED) {
        return;
    }
    rb_gi_arg_metadata_free(metadata);
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
    if (info) {
        args->namespace = g_base_info_get_namespace(info);
        if (GI_IS_FUNCTION_INFO(info)) {
            args->name = g_function_info_get_symbol((GIFunctionInfo *)info);
        } else {
            args->name = g_base_info_get_name(info);
        }
    } else {
        args->namespace = NULL;
        args->name = NULL;
    }
    args->rb_receiver = rb_receiver;
    args->receiver_type_class = NULL;
    args->rb_args = rb_args;
    args->raw_args = raw_args;
    if (NIL_P(rb_args) && !raw_args) {
        args->in_args = NULL;
        args->out_args = NULL;
        args->metadata = NULL;
    } else {
        args->in_args = g_array_new(FALSE, FALSE, sizeof(GIArgument));
        args->out_args = g_array_new(FALSE, FALSE, sizeof(GIArgument));
        args->metadata =
            g_ptr_array_new_with_free_func(rb_gi_arguments_metadata_free);
    }
    args->rb_mode_p = !(NIL_P(rb_args));

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
    } else if (raw_args) {
        rb_gi_arguments_fill_raw_args(args);
    }
}

void
rb_gi_arguments_clear(RBGIArguments *args)
{
    rb_gi_arguments_in_clear(args);
    rb_gi_arguments_out_clear(args);

    if (args->receiver_type_class) {
        g_type_class_unref(args->receiver_type_class);
    }
    if (args->in_args) {
        g_array_unref(args->in_args);
    }
    if (args->out_args) {
        g_array_unref(args->out_args);
    }
    if (args->metadata) {
        g_ptr_array_unref(args->metadata);
    }
}

VALUE
rb_gi_arguments_get_rb_in_args(RBGIArguments *args)
{
    return rb_gi_arguments_in_to_ruby(args);
}

VALUE
rb_gi_arguments_get_rb_out_args(RBGIArguments *args)
{
    return rb_gi_arguments_out_to_ruby(args);
}

typedef struct {
    RBGIArguments *args;
    GIArgument *arg;
    RBGIArgMetadata *arg_metadata;
    gboolean duplicate;
    GIInterfaceInfo *interface_info;
} InterfaceToRubyData;

static VALUE
rb_gi_arguments_convert_arg_interface_body(VALUE user_data)
{
    InterfaceToRubyData *data = (InterfaceToRubyData *)user_data;
    GIInfoType interface_type = g_base_info_get_type(data->interface_info);
    GType gtype = g_registered_type_info_get_g_type(data->interface_info);
    switch (interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[%s] -> Ruby",
                 g_info_type_to_string(interface_type));
        return Qnil;
      case GI_INFO_TYPE_STRUCT:
        return rb_gi_struct_info_to_ruby(data->interface_info,
                                         data->arg->v_pointer,
                                         !data->duplicate);
      case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[%s] -> Ruby",
                 g_info_type_to_string(interface_type));
        return Qnil;
      case GI_INFO_TYPE_ENUM:
        if (gtype == G_TYPE_NONE) {
            return INT2NUM(data->arg->v_int32);
        } else {
            return GENUM2RVAL(data->arg->v_int32, gtype);
        }
      case GI_INFO_TYPE_FLAGS:
        if (gtype == G_TYPE_NONE) {
            return INT2NUM(data->arg->v_int32);
        } else {
            return GFLAGS2RVAL(data->arg->v_int32, gtype);
        }
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        if (!G_IS_OBJECT(data->arg->v_pointer)) {
            GIObjectInfoRefFunction ref = g_object_info_get_ref_function_pointer(data->arg_metadata->type.interface_info);
            if (ref) {
                ref(data->arg->v_pointer);
            }
        }
        return GOBJ2RVAL(data->arg->v_pointer);
      case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[%s] -> Ruby",
                 g_info_type_to_string(interface_type));
        return Qnil;
      case GI_INFO_TYPE_INVALID_0:
        g_assert_not_reached();
        return Qnil;
      case GI_INFO_TYPE_UNION:
        return BOXED2RVAL(data->arg->v_pointer, gtype);
      case GI_INFO_TYPE_VALUE:
      case GI_INFO_TYPE_SIGNAL:
      case GI_INFO_TYPE_VFUNC:
      case GI_INFO_TYPE_PROPERTY:
      case GI_INFO_TYPE_FIELD:
      case GI_INFO_TYPE_ARG:
      case GI_INFO_TYPE_TYPE:
      case GI_INFO_TYPE_UNRESOLVED:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[%s] -> Ruby",
                 g_info_type_to_string(interface_type));
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_interface_ensure(VALUE user_data)
{
    InterfaceToRubyData *data = (InterfaceToRubyData *)user_data;

    if (data->interface_info) {
        g_base_info_unref(data->interface_info);
    }

    return Qnil;
}

static VALUE
rb_gi_arguments_convert_arg_interface(RBGIArguments *args,
                                      GIArgument *arg,
                                      RBGIArgMetadata *arg_metadata,
                                      gboolean duplicate)
{
    InterfaceToRubyData data;
    data.args = args;
    data.arg = arg;
    data.arg_metadata = arg_metadata;
    data.duplicate = duplicate;
    data.interface_info = g_type_info_get_interface(arg_metadata->type_info);
    return rb_ensure(rb_gi_arguments_convert_arg_interface_body, (VALUE)&data,
                     rb_gi_arguments_convert_arg_interface_ensure, (VALUE)&data);
}

typedef struct {
    RBGIArguments *args;
    GIArgument *arg;
    RBGIArgMetadata *arg_metadata;
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;
    GITypeInfo *interface_type_info;
} ArrayLikeToRubyData;

static VALUE
rb_gi_arguments_convert_arg_array_like_ensure(VALUE user_data)
{
    ArrayLikeToRubyData *data = (ArrayLikeToRubyData *)user_data;

    if (data->element_type_info) {
        g_base_info_unref(data->element_type_info);
    }
    if (data->interface_type_info) {
        g_base_info_unref(data->interface_type_info);
    }

    return Qnil;
}

gint64
rb_gi_argument_out_array_get_length(GIArgument *arg,
                                    RBGIArgMetadata *metadata,
                                    gboolean is_pointer)
{
    switch (metadata->type.tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid out array length argument?: <%s>",
                 g_type_tag_to_string(metadata->type.tag));
        return -1;
      case GI_TYPE_TAG_INT8:
        if (is_pointer) {
            return *((gint8 *)(arg->v_pointer));
        } else {
            return arg->v_int8;
        }
      case GI_TYPE_TAG_UINT8:
        if (is_pointer) {
            return *((guint8 *)(arg->v_pointer));
        } else {
            return arg->v_uint8;
        }
      case GI_TYPE_TAG_INT16:
        if (is_pointer) {
            return *((gint16 *)(arg->v_pointer));
        } else {
            return arg->v_int16;
        }
      case GI_TYPE_TAG_UINT16:
        if (is_pointer) {
            return *((guint16 *)(arg->v_pointer));
        } else {
            return arg->v_uint16;
        }
      case GI_TYPE_TAG_INT32:
        if (is_pointer) {
            return *((gint32 *)(arg->v_pointer));
        } else {
            return arg->v_int32;
        }
      case GI_TYPE_TAG_UINT32:
        if (is_pointer) {
            return *((guint32 *)(arg->v_pointer));
        } else {
            return arg->v_uint32;
        }
      case GI_TYPE_TAG_INT64:
        if (is_pointer) {
            return *((gint64 *)(arg->v_pointer));
        } else {
            return arg->v_int64;
        }
      case GI_TYPE_TAG_UINT64:
        if (is_pointer) {
            return *((guint64 *)(arg->v_pointer));
        } else {
            return arg->v_uint64;
        }
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
                 "TODO: invalid out array length argument?: <%s>",
                 g_type_tag_to_string(metadata->type.tag));
        return -1;
      default:
        g_assert_not_reached();
        return -1;
    }
}

static gint64
rb_gi_arguments_convert_arg_array_body_get_length(ArrayLikeToRubyData *data)
{
    GITypeInfo *type_info = data->arg_metadata->type_info;
    gint length_index = g_type_info_get_array_length(type_info);
    if (length_index == -1) {
        return -1;
    }

    if (!data->args->metadata && data->arg_metadata->struct_info) {
        GIFieldInfo *field_info =
            g_struct_info_get_field(data->arg_metadata->struct_info,
                                    length_index);
        GIArgument value;
        g_field_info_get_field(field_info,
                               data->arg_metadata->struct_memory,
                               &value);
        RBGIArgMetadata length_metadata;
        GITypeInfo *length_type_info = g_field_info_get_type(field_info);
        rb_gi_arg_metadata_init_type_info(&length_metadata, length_type_info);
        rb_gi_arg_metadata_init_struct_info(&length_metadata,
                                            data->arg_metadata->struct_info,
                                            data->arg_metadata->struct_memory);
        int64_t length =
            rb_gi_argument_out_array_get_length(&value,
                                                &length_metadata,
                                                FALSE);
        /* TODO: Use ensure */
        rb_gi_arg_metadata_clear(&length_metadata);
        g_base_info_unref(field_info);
        return length;
    }

    GIArgument *length_arg = NULL;
    RBGIArgMetadata *length_metadata =
        g_ptr_array_index(data->args->metadata, length_index);
    if (length_metadata->direction == GI_DIRECTION_OUT) {
        length_arg = &g_array_index(data->args->out_args,
                                    GIArgument,
                                    length_metadata->out_arg_index);
    } else if (length_metadata->direction == GI_DIRECTION_INOUT) {
        length_arg = &g_array_index(data->args->in_args,
                                    GIArgument,
                                    length_metadata->in_arg_index);
    }

    if (length_arg) {
        gboolean is_pointer =
            !(length_metadata->array_metadata &&
              length_metadata->array_metadata->output_buffer_p);
        return rb_gi_argument_out_array_get_length(length_arg,
                                                   length_metadata,
                                                   is_pointer);
    } else {
        length_arg = &g_array_index(data->args->in_args,
                                    GIArgument,
                                    length_metadata->in_arg_index);
        return rb_gi_argument_out_array_get_length(length_arg,
                                                   length_metadata,
                                                   FALSE);
    }
}

static VALUE
rb_gi_arguments_convert_arg_array_body_c_sized_interface(
    ArrayLikeToRubyData *data,
    gint64 length,
    const char *array_c_type)
{
    gconstpointer *elements = data->arg->v_pointer;
    data->interface_type_info =
        g_type_info_get_interface(data->element_type_info);
    GIInfoType type = g_base_info_get_type(data->interface_type_info);
    GType gtype = g_registered_type_info_get_g_type(data->interface_type_info);
    switch (type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s][interface(%s)](%s) -> Ruby",
                 array_c_type,
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        return Qnil;
      case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg,
                            rb_gi_struct_info_to_ruby(data->interface_type_info,
                                                      (gpointer)elements[i],
                                                      TRUE));
            }
            return rb_arg;
        } else {
            /* TODO: Should we check caller_allocates? */
            gsize struct_size =
                g_struct_info_get_size(data->interface_type_info);
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                gpointer element = ((gchar *)elements) + struct_size * i;
                rb_ary_push(rb_arg, BOXED2RVAL(element, gtype));
            }
            return rb_arg;
        }
      case GI_INFO_TYPE_BOXED:
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s][interface(%s)](%s) -> Ruby",
                 array_c_type,
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        return Qnil;
      case GI_INFO_TYPE_OBJECT:
        {
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, GOBJ2RVAL((GObject *)(elements[i])));
            }
            return rb_arg;
        }
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
                 "TODO: GIArgument(array)[c][%s][interface(%s)](%s) -> Ruby",
                 array_c_type,
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_array_body_c_sized(ArrayLikeToRubyData *data,
                                               gint64 length)
{
    const char *array_c_type = "length";
    gconstpointer elements = data->arg->v_pointer;
    switch (data->element_type_tag) {
      case GI_TYPE_TAG_VOID:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s][%s] -> Ruby",
                 array_c_type,
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      case GI_TYPE_TAG_BOOLEAN:
        {
            const gboolean *booleans = (const gboolean *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, CBOOL2RVAL(booleans[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_INT8:
        {
            const gint8 *numbers = (const gint8 *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, INT2NUM(numbers[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_UINT8:
        if (data->arg_metadata->input_buffer_p) {
            VALUE rb_arg = rb_str_new_static(elements, length);
            rb_str_freeze(rb_arg);
            return rb_arg;
        } else {
            const guint8 *numbers = (const guint8 *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, UINT2NUM(numbers[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_INT16:
        {
            const gint16 *numbers = (const gint16 *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, INT2NUM(numbers[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_UINT16:
        {
            const guint16 *numbers = (const guint16 *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, UINT2NUM(numbers[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_INT32:
        {
            const gint32 *numbers = (const gint32 *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, INT2NUM(numbers[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_UINT32:
        {
            const guint32 *numbers = (const guint32 *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, UINT2NUM(numbers[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_INT64:
        {
            const gint64 *numbers = (const gint64 *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, LL2NUM(numbers[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_UINT64:
        {
            const guint64 *numbers = (const guint64 *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, ULL2NUM(numbers[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_FLOAT:
        {
            const gfloat *numbers = (const gfloat *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, rb_float_new(numbers[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_DOUBLE:
        {
            const gdouble *numbers = (const gdouble *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, rb_float_new(numbers[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_GTYPE:
        {
            const GType *types = (const GType *)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, rbgobj_gtype_new(types[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_UTF8:
        {
            const gchar **strings = (const gchar **)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, CSTR2RVAL(strings[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_FILENAME:
        {
            const gchar **filenames = (const gchar **)elements;
            VALUE rb_arg = rb_ary_new_capa(length);
            gint64 i;
            for (i = 0; i < length; i++) {
                rb_ary_push(rb_arg, CSTRFILENAME2RVAL(filenames[i]));
            }
            return rb_arg;
        }
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s][%s] -> Ruby",
                 array_c_type,
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      case GI_TYPE_TAG_INTERFACE:
        return rb_gi_arguments_convert_arg_array_body_c_sized_interface(
            data, length, array_c_type);
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s][%s] -> Ruby",
                 array_c_type,
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_array_body_c_fixed_size(ArrayLikeToRubyData *data,
                                                    gint fixed_size)
{
    const char *array_c_type = "fixed-size";
    switch (data->element_type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s][%s] -> Ruby",
                 array_c_type,
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      case GI_TYPE_TAG_INT8:
        {
            gint8 *array = (gint8 *)(data->arg->v_pointer);
            VALUE rb_array = rb_ary_new_capa(fixed_size);
            gint i;
            for (i = 0; i < fixed_size; i++) {
                rb_ary_push(rb_array, RB_INT2NUM(array[i]));
            }
            return rb_array;
        }
      case GI_TYPE_TAG_UINT8:
        {
            guint8 *array = (guint8 *)(data->arg->v_pointer);
            VALUE rb_array = rb_ary_new_capa(fixed_size);
            gint i;
            for (i = 0; i < fixed_size; i++) {
                rb_ary_push(rb_array, RB_UINT2NUM(array[i]));
            }
            return rb_array;
        }
      case GI_TYPE_TAG_INT16:
        {
            gint16 *array = (gint16 *)(data->arg->v_pointer);
            VALUE rb_array = rb_ary_new_capa(fixed_size);
            gint i;
            for (i = 0; i < fixed_size; i++) {
                rb_ary_push(rb_array, RB_INT2NUM(array[i]));
            }
            return rb_array;
        }
      case GI_TYPE_TAG_UINT16:
        {
            guint16 *array = (guint16 *)(data->arg->v_pointer);
            VALUE rb_array = rb_ary_new_capa(fixed_size);
            gint i;
            for (i = 0; i < fixed_size; i++) {
                rb_ary_push(rb_array, RB_UINT2NUM(array[i]));
            }
            return rb_array;
        }
      case GI_TYPE_TAG_INT32:
        {
            gint32 *array = (gint32 *)(data->arg->v_pointer);
            VALUE rb_array = rb_ary_new_capa(fixed_size);
            gint i;
            for (i = 0; i < fixed_size; i++) {
                rb_ary_push(rb_array, RB_INT2NUM(array[i]));
            }
            return rb_array;
        }
      case GI_TYPE_TAG_UINT32:
        {
            guint32 *array = (guint32 *)(data->arg->v_pointer);
            VALUE rb_array = rb_ary_new_capa(fixed_size);
            gint i;
            for (i = 0; i < fixed_size; i++) {
                rb_ary_push(rb_array, RB_UINT2NUM(array[i]));
            }
            return rb_array;
        }
      case GI_TYPE_TAG_INT64:
        {
            gint64 *array = (gint64 *)(data->arg->v_pointer);
            VALUE rb_array = rb_ary_new_capa(fixed_size);
            gint i;
            for (i = 0; i < fixed_size; i++) {
                rb_ary_push(rb_array, RB_LL2NUM(array[i]));
            }
            return rb_array;
        }
      case GI_TYPE_TAG_UINT64:
        {
            guint64 *array = (guint64 *)(data->arg->v_pointer);
            VALUE rb_array = rb_ary_new_capa(fixed_size);
            gint i;
            for (i = 0; i < fixed_size; i++) {
                rb_ary_push(rb_array, RB_ULL2NUM(array[i]));
            }
            return rb_array;
        }
      case GI_TYPE_TAG_FLOAT:
        {
            float *array = (float *)(data->arg->v_pointer);
            VALUE rb_array = rb_ary_new_capa(fixed_size);
            gint i;
            for (i = 0; i < fixed_size; i++) {
                rb_ary_push(rb_array, rb_float_new(array[i]));
            }
            return rb_array;
        }
      case GI_TYPE_TAG_DOUBLE:
        {
            double *array = (double *)(data->arg->v_pointer);
            VALUE rb_array = rb_ary_new_capa(fixed_size);
            gint i;
            for (i = 0; i < fixed_size; i++) {
                rb_ary_push(rb_array, rb_float_new(array[i]));
            }
            return rb_array;
        }
      case GI_TYPE_TAG_GTYPE:
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s][%s] -> Ruby",
                 array_c_type,
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      case GI_TYPE_TAG_INTERFACE:
        return rb_gi_arguments_convert_arg_array_body_c_sized_interface(
            data, fixed_size, array_c_type);
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s][%s] -> Ruby",
                 array_c_type,
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_array_body_c(ArrayLikeToRubyData *data,
                                         gint64 length)
{
    gconstpointer *elements = data->arg->v_pointer;
    if (!elements) {
        return rb_ary_new();
    }

    GITypeInfo *type_info = data->arg_metadata->type_info;
    gint fixed_size = g_type_info_get_array_fixed_size(type_info);
    gboolean zero_terminated_p = g_type_info_is_zero_terminated(type_info);

    if (length != -1) {
        return rb_gi_arguments_convert_arg_array_body_c_sized(data, length);
    } else if (zero_terminated_p) {
        return STRV2RVAL((const gchar **)elements);
    } else if (fixed_size != -1) {
        return rb_gi_arguments_convert_arg_array_body_c_fixed_size(data,
                                                                   fixed_size);
    } else {
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c] -> Ruby: "
                 "zero-terminated: %s "
                 "fixed-size: %d "
                 "length: %" G_GINT64_FORMAT,
                 zero_terminated_p ? "true" : "false",
                 fixed_size,
                 length);
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_array_body_array_interface(ArrayLikeToRubyData *data)
{
    GArray *elements = data->arg->v_pointer;
    data->interface_type_info =
        g_type_info_get_interface(data->element_type_info);
    GIInfoType type = g_base_info_get_type(data->interface_type_info);
    GType gtype = g_registered_type_info_get_g_type(data->interface_type_info);
    switch (type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[array][interface(%s)](%s) -> Ruby",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        return Qnil;
      case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            VALUE rb_arg = rb_ary_new_capa(elements->len);
            guint element_size = g_array_get_element_size(elements);
            guint i;
            for (i = 0; i < elements->len; i++) {
                gpointer element;
                element = elements->data + (element_size * i);
                rb_ary_push(rb_arg,
                            rb_gi_struct_info_to_ruby(data->interface_type_info,
                                                      element,
                                                      FALSE));
            }
            return rb_arg;
        } else {
            rb_raise(rb_eNotImpError,
                     "TODO: GIArgument(array)[array][interface(%s)](%s) -> Ruby",
                     g_info_type_to_string(type),
                     g_type_name(gtype));
            return Qnil;
        }
      case GI_INFO_TYPE_BOXED:
      case GI_INFO_TYPE_ENUM:
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
                 "TODO: GIArgument(array)[array][interface(%s)](%s) -> Ruby",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_array_body_array(ArrayLikeToRubyData *data)
{
    GArray *elements = data->arg->v_pointer;
    if (!elements) {
        return Qnil;
    }

    switch (data->element_type_tag) {
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
                 "TODO: GIArgument(array)[array][%s] -> Ruby",
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      case GI_TYPE_TAG_INTERFACE:
        return rb_gi_arguments_convert_arg_array_body_array_interface(data);
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[array][%s] -> Ruby",
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_array_body(VALUE user_data)
{
    ArrayLikeToRubyData *data = (ArrayLikeToRubyData *)user_data;
    GITypeInfo *type_info = data->arg_metadata->type_info;
    GIArrayType array_type = g_type_info_get_array_type(type_info);
    switch (array_type) {
      case GI_ARRAY_TYPE_C:
        {
            gint64 length =
                rb_gi_arguments_convert_arg_array_body_get_length(data);
            return rb_gi_arguments_convert_arg_array_body_c(data, length);
        }
      case GI_ARRAY_TYPE_ARRAY:
        return rb_gi_arguments_convert_arg_array_body_array(data);
      case GI_ARRAY_TYPE_PTR_ARRAY:
        rb_raise(rb_eNotImpError, "TODO: GIArgument(array)[ptr-array] -> Ruby");
        return Qnil;
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        rb_raise(rb_eNotImpError, "TODO: GIArgument(array)[byte-array] -> Ruby");
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_array(RBGIArguments *args,
                                  GIArgument *arg,
                                  RBGIArgMetadata *arg_metadata)
{
    ArrayLikeToRubyData data;
    data.args = args;
    data.arg = arg;
    data.arg_metadata = arg_metadata;
    data.element_type_info =
        g_type_info_get_param_type(arg_metadata->type_info, 0);
    data.element_type_tag = g_type_info_get_tag(data.element_type_info);
    data.interface_type_info = NULL;
    return rb_ensure(rb_gi_arguments_convert_arg_array_body,
                     (VALUE)&data,
                     rb_gi_arguments_convert_arg_array_like_ensure,
                     (VALUE)&data);
}

static VALUE
rb_gi_arguments_convert_arg_glist_body_interface(ArrayLikeToRubyData *data)
{
    data->interface_type_info =
        g_type_info_get_interface(data->element_type_info);
    GIInfoType type = g_base_info_get_type(data->interface_type_info);
    GType gtype = g_registered_type_info_get_g_type(data->interface_type_info);
    switch (type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GList)[interface(%s)](%s) -> Ruby",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        return Qnil;
      case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            VALUE rb_arg = rb_ary_new();
            GList *node;
            for (node = data->arg->v_pointer; node; node = g_list_next(node)) {
                rb_ary_push(rb_arg,
                            rb_gi_struct_info_to_ruby(data->interface_type_info,
                                                      node->data,
                                                      TRUE));
            }
            return rb_arg;
        } else if (gtype == G_TYPE_VARIANT) {
            VALUE rb_arg = rb_ary_new();
            GList *node;
            for (node = data->arg->v_pointer; node; node = g_list_next(node)) {
                rb_ary_push(rb_arg, rbg_variant_to_ruby(node->data));
            }
            return rb_arg;
        } else {
            return BOXEDGLIST2RVAL(data->arg->v_pointer, gtype);
        }
      case GI_INFO_TYPE_BOXED:
        return BOXEDGLIST2RVAL(data->arg->v_pointer, gtype);
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        return GOBJGLIST2RVAL(data->arg->v_pointer);
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
                 "TODO: GIArgument(GList)[interface(%s)](%s) -> Ruby",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_glist_body(VALUE user_data)
{
    ArrayLikeToRubyData *data = (ArrayLikeToRubyData *)user_data;

    switch (data->element_type_tag) {
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
                 "TODO: GIArgument(GList)[%s] -> Ruby",
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      case GI_TYPE_TAG_UTF8:
        return CSTRGLIST2RVAL(data->arg->v_pointer);
      case GI_TYPE_TAG_FILENAME:
        return FILENAMEGLIST2RVAL(data->arg->v_pointer);
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GList)[%s] -> Ruby",
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      case GI_TYPE_TAG_INTERFACE:
        return rb_gi_arguments_convert_arg_glist_body_interface(data);
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GList)[%s] -> Ruby",
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_glist(RBGIArguments *args,
                                  GIArgument *arg,
                                  RBGIArgMetadata *arg_metadata)
{
    ArrayLikeToRubyData data;
    data.args = args;
    data.arg = arg;
    data.arg_metadata = arg_metadata;
    data.element_type_info =
        g_type_info_get_param_type(arg_metadata->type_info, 0);
    data.element_type_tag = g_type_info_get_tag(data.element_type_info);
    data.interface_type_info = NULL;
    return rb_ensure(rb_gi_arguments_convert_arg_glist_body,
                     (VALUE)&data,
                     rb_gi_arguments_convert_arg_array_like_ensure,
                     (VALUE)&data);
}

static VALUE
rb_gi_arguments_convert_arg_gslist_body_interface(ArrayLikeToRubyData *data)
{
    data->interface_type_info =
        g_type_info_get_interface(data->element_type_info);
    GIInfoType type = g_base_info_get_type(data->interface_type_info);
    GType gtype = g_registered_type_info_get_g_type(data->interface_type_info);
    switch (type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GSList)[interface(%s)](%s) -> Ruby",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        return Qnil;
      case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            VALUE rb_arg = rb_ary_new();
            GSList *node;
            for (node = data->arg->v_pointer; node; node = g_slist_next(node)) {
                rb_ary_push(rb_arg,
                            rb_gi_struct_info_to_ruby(data->interface_type_info,
                                                      node->data,
                                                      TRUE));
            }
            return rb_arg;
        } else if (gtype == G_TYPE_VARIANT) {
            VALUE rb_arg = rb_ary_new();
            GSList *node;
            for (node = data->arg->v_pointer; node; node = g_slist_next(node)) {
                rb_ary_push(rb_arg, rbg_variant_to_ruby(node->data));
            }
            return rb_arg;
        } else {
            return BOXEDGSLIST2RVAL(data->arg->v_pointer, gtype);
        }
      case GI_INFO_TYPE_BOXED:
        return BOXEDGSLIST2RVAL(data->arg->v_pointer, gtype);
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        return GOBJGSLIST2RVAL(data->arg->v_pointer);
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
                 "TODO: GIArgument(GSList)[interface(%s)](%s) -> Ruby",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_gslist_body(VALUE user_data)
{
    ArrayLikeToRubyData *data = (ArrayLikeToRubyData *)user_data;

    switch (data->element_type_tag) {
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
                 "TODO: GIArgument(GSList)[%s] -> Ruby",
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      case GI_TYPE_TAG_UTF8:
        return CSTRGSLIST2RVAL(data->arg->v_pointer);
      case GI_TYPE_TAG_FILENAME:
        return FILENAMEGSLIST2RVAL(data->arg->v_pointer);
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GSList)[%s] -> Ruby",
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      case GI_TYPE_TAG_INTERFACE:
        return rb_gi_arguments_convert_arg_gslist_body_interface(data);
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GSList)[%s] -> Ruby",
                 g_type_tag_to_string(data->element_type_tag));
        return Qnil;
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

static VALUE
rb_gi_arguments_convert_arg_gslist(RBGIArguments *args,
                                   GIArgument *arg,
                                   RBGIArgMetadata *arg_metadata)
{
    ArrayLikeToRubyData data;
    data.args = args;
    data.arg = arg;
    data.arg_metadata = arg_metadata;
    data.element_type_info =
        g_type_info_get_param_type(arg_metadata->type_info, 0);
    data.element_type_tag = g_type_info_get_tag(data.element_type_info);
    data.interface_type_info = NULL;
    return rb_ensure(rb_gi_arguments_convert_arg_gslist_body,
                     (VALUE)&data,
                     rb_gi_arguments_convert_arg_array_like_ensure,
                     (VALUE)&data);
}

typedef struct {
    RBGIArguments *args;
    GIArgument *arg;
    RBGIArgMetadata *arg_metadata;
    VALUE rb_table;
    RBGIArgMetadata *key_metadata;
    RBGIArgMetadata *value_metadata;
} GHashToRubyData;

static void
rb_gi_arguments_convert_arg_ghash_foreach_body(gpointer key,
                                               gpointer value,
                                               gpointer user_data)
{
    GHashToRubyData *data = user_data;
    VALUE rb_key;
    VALUE rb_value;

    switch (data->key_metadata->type.tag) {
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
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_metadata->type.tag),
                 g_type_tag_to_string(data->value_metadata->type.tag));
        break;
      case GI_TYPE_TAG_UTF8:
        rb_key = CSTR2RVAL(key);
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_metadata->type.tag),
                 g_type_tag_to_string(data->value_metadata->type.tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        {
            GIArgument key_arg;
            key_arg.v_pointer = key;
            rb_key = rb_gi_arguments_convert_arg_interface(data->args,
                                                           &key_arg,
                                                           data->key_metadata,
                                                           FALSE);
        }
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_metadata->type.tag),
                 g_type_tag_to_string(data->value_metadata->type.tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    switch (data->value_metadata->type.tag) {
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
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_metadata->type.tag),
                 g_type_tag_to_string(data->value_metadata->type.tag));
        break;
      case GI_TYPE_TAG_UTF8:
        rb_value = CSTR2RVAL(value);
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_metadata->type.tag),
                 g_type_tag_to_string(data->value_metadata->type.tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        {
            GIArgument value_arg;
            value_arg.v_pointer = value;
            rb_value = rb_gi_arguments_convert_arg_interface(
                data->args,
                &value_arg,
                data->value_metadata,
                FALSE);
        }
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_metadata->type.tag),
                 g_type_tag_to_string(data->value_metadata->type.tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    rb_hash_aset(data->rb_table, rb_key, rb_value);
}

static VALUE
rb_gi_arguments_convert_arg_ghash_body(VALUE user_data)
{
    GHashToRubyData *data = (GHashToRubyData *)user_data;
    g_hash_table_foreach(data->arg->v_pointer,
                         rb_gi_arguments_convert_arg_ghash_foreach_body,
                         data);
    return data->rb_table;
}

static VALUE
rb_gi_arguments_convert_arg_ghash_ensure(VALUE user_data)
{
    GHashToRubyData *data = (GHashToRubyData *)user_data;
    rb_gi_arg_metadata_clear(data->key_metadata);
    rb_gi_arg_metadata_clear(data->value_metadata);
    return Qnil;
}

static VALUE
rb_gi_arguments_convert_arg_ghash(RBGIArguments *args,
                                  GIArgument *arg,
                                  RBGIArgMetadata *arg_metadata)
{
    if (arg_metadata->may_be_null_p && !arg->v_pointer) {
        return Qnil;
    }

    GHashToRubyData data;

    data.args = args;
    data.arg = arg;
    data.arg_metadata = arg_metadata;
    data.rb_table = rb_hash_new();

    GITypeInfo *type_info = arg_metadata->type_info;

    RBGIArgMetadata key_metadata;
    rb_gi_arg_metadata_init_type_info(&key_metadata,
                                      g_type_info_get_param_type(type_info, 0));
    rb_gi_arg_metadata_init_struct_info(&key_metadata, NULL, NULL);
    data.key_metadata = &key_metadata;

    RBGIArgMetadata value_metadata;
    rb_gi_arg_metadata_init_type_info(&value_metadata,
                                      g_type_info_get_param_type(type_info, 1));
    rb_gi_arg_metadata_init_struct_info(&value_metadata, NULL, NULL);
    data.value_metadata = &value_metadata;

    return rb_ensure(rb_gi_arguments_convert_arg_ghash_body, (VALUE)&data,
                     rb_gi_arguments_convert_arg_ghash_ensure, (VALUE)&data);
}

static VALUE
rb_gi_arguments_convert_arg_unichar(RBGIArguments *args,
                                    GIArgument *arg,
                                    RBGIArgMetadata *arg_metadata)
{
    GError *error = NULL;
    gunichar ucs4_character = arg->v_uint32;
    gchar *utf8_string = g_ucs4_to_utf8(&ucs4_character, 1, NULL, NULL, &error);
    if (error) {
        RG_RAISE_ERROR(error);
    }
    return CSTR2RVAL_FREE(utf8_string);
}

VALUE
rb_gi_arguments_convert_arg(RBGIArguments *args,
                            GIArgument *arg,
                            RBGIArgMetadata *arg_metadata,
                            gboolean duplicate)
{
    GITypeTag type_tag = g_type_info_get_tag(arg_metadata->type_info);
    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
        if (g_type_info_is_pointer(arg_metadata->type_info)) {
            return POINTER2NUM(arg->v_pointer);
        } else {
            return Qnil;
        }
      case GI_TYPE_TAG_BOOLEAN:
        return CBOOL2RVAL(arg->v_boolean);
      case GI_TYPE_TAG_INT8:
        return INT2NUM(arg->v_int8);
      case GI_TYPE_TAG_UINT8:
        return UINT2NUM(arg->v_uint8);
      case GI_TYPE_TAG_INT16:
        return INT2NUM(arg->v_int16);
      case GI_TYPE_TAG_UINT16:
        return UINT2NUM(arg->v_uint16);
      case GI_TYPE_TAG_INT32:
        return INT2NUM(arg->v_int32);
      case GI_TYPE_TAG_UINT32:
        return UINT2NUM(arg->v_uint32);
      case GI_TYPE_TAG_INT64:
        return LL2NUM(arg->v_int64);
      case GI_TYPE_TAG_UINT64:
        return ULL2NUM(arg->v_uint64);
      case GI_TYPE_TAG_FLOAT:
        return DBL2NUM(arg->v_float);
      case GI_TYPE_TAG_DOUBLE:
        return DBL2NUM(arg->v_double);
      case GI_TYPE_TAG_GTYPE:
        if (arg->v_size == G_TYPE_INVALID) {
            return Qnil;
        } else {
            return rbgobj_gtype_new(arg->v_size);
        }
      case GI_TYPE_TAG_UTF8:
        return CSTR2RVAL(arg->v_string);
      case GI_TYPE_TAG_FILENAME:
        return CSTRFILENAME2RVAL(arg->v_string);
      case GI_TYPE_TAG_ARRAY:
        return rb_gi_arguments_convert_arg_array(args, arg, arg_metadata);
      case GI_TYPE_TAG_INTERFACE:
        return rb_gi_arguments_convert_arg_interface(args,
                                                     arg,
                                                     arg_metadata,
                                                     duplicate);
      case GI_TYPE_TAG_GLIST:
        return rb_gi_arguments_convert_arg_glist(args, arg, arg_metadata);
      case GI_TYPE_TAG_GSLIST:
        return rb_gi_arguments_convert_arg_gslist(args, arg, arg_metadata);
      case GI_TYPE_TAG_GHASH:
        return rb_gi_arguments_convert_arg_ghash(args, arg, arg_metadata);
      case GI_TYPE_TAG_ERROR:
        return GERROR2RVAL(arg->v_pointer);
      case GI_TYPE_TAG_UNICHAR:
        return rb_gi_arguments_convert_arg_unichar(args, arg, arg_metadata);
      default:
        g_assert_not_reached();
        return Qnil;
    }
}

typedef struct {
    RBGIArguments *args;
    GIArgument *value;
    RBGIArgMetadata *metadata;
} ReturnValueToRubyData;

static VALUE
rb_gi_arguments_convert_return_value_body(VALUE user_data)
{
    ReturnValueToRubyData *data = (ReturnValueToRubyData *)user_data;
    return rb_gi_arguments_convert_arg(data->args,
                                       data->value,
                                       data->metadata,
                                       FALSE);
}

static void
rb_gi_arguments_convert_return_value_free_container(ReturnValueToRubyData *data)
{
    GITypeTag type_tag = data->metadata->type.tag;
    switch (type_tag) {
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
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) as container",
                 g_type_tag_to_string(type_tag));
        break;
      case GI_TYPE_TAG_ARRAY:
        g_free(data->value->v_pointer);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) as container",
                 g_type_tag_to_string(type_tag));
        break;
      case GI_TYPE_TAG_GLIST:
        g_list_free(data->value->v_pointer);
        break;
      case GI_TYPE_TAG_GSLIST:
        g_slist_free(data->value->v_pointer);
        break;
      case GI_TYPE_TAG_GHASH:
        g_hash_table_unref(data->value->v_pointer);
        break;
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) as container",
                 g_type_tag_to_string(type_tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_convert_return_value_free_everything_array_c(
    ReturnValueToRubyData *data)
{
    GITypeTag element_type_tag = data->metadata->element_type.tag;
    switch (element_type_tag) {
      case GI_TYPE_TAG_VOID:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(array)[c][%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
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
        g_free(data->value->v_pointer);
        break;
      case GI_TYPE_TAG_GTYPE:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(array)[c][%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
        g_strfreev(data->value->v_pointer);
        break;
      case GI_TYPE_TAG_FILENAME:
        g_strfreev(data->value->v_pointer);
        break;
      case GI_TYPE_TAG_ARRAY:
      case GI_TYPE_TAG_INTERFACE:
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(array)[c][%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_convert_return_value_free_everything_array(
    ReturnValueToRubyData *data)
{
    switch (data->metadata->array_type) {
      case GI_ARRAY_TYPE_C:
        rb_gi_arguments_convert_return_value_free_everything_array_c(data);
        break;
      case GI_ARRAY_TYPE_ARRAY:
        g_array_free(data->value->v_pointer, TRUE);
        break;
      case GI_ARRAY_TYPE_PTR_ARRAY:
        g_ptr_array_free(data->value->v_pointer, TRUE);
        break;
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        g_byte_array_free(data->value->v_pointer, TRUE);
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_convert_return_value_free_everything_interface(
    ReturnValueToRubyData *data)
{
    if (!data->value->v_pointer) {
        return;
    }

    GIInfoType type = data->metadata->type.interface_type;
    GType gtype = data->metadata->type.interface_gtype;
    switch (type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface(%s))(%s) everything",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        break;
      case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_INVALID) {
            xfree(data->value->v_pointer);
        } else if (gtype == G_TYPE_VARIANT) {
            g_variant_unref(data->value->v_pointer);
        } else {
            if (G_TYPE_IS_BOXED(gtype)) {
                g_boxed_free(gtype, data->value->v_pointer);
            } else {
                rbgobj_instance_unref(data->value->v_pointer);
            }
        }
        break;
      case GI_INFO_TYPE_BOXED:
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface(%s))(%s) everything",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        break;
      case GI_INFO_TYPE_OBJECT:
        {
            GObject *object = data->value->v_pointer;
            if (G_IS_OBJECT(object)) {
                if (g_object_is_floating(object)) {
                    g_object_ref_sink(object);
                }
                g_object_unref(object);
            } else {
                GIObjectInfoUnrefFunction unref = g_object_info_get_unref_function_pointer(data->metadata->type.interface_info);
                if (unref) {
                    unref(object);
                }
            }
        }
        break;
      case GI_INFO_TYPE_INTERFACE:
        g_object_unref(data->value->v_pointer);
        break;
      case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface(%s))(%s) everything",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        break;
      case GI_INFO_TYPE_INVALID_0:
        g_assert_not_reached();
        break;
      case GI_INFO_TYPE_UNION:
        if (gtype == G_TYPE_NONE) {
            rb_raise(rb_eNotImpError,
                     "TODO: free GIArgument(interface(%s))(%s) everything",
                     g_info_type_to_string(type),
                     g_type_name(gtype));
        } else {
            g_boxed_free(gtype, data->value->v_pointer);
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
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface(%s))(%s) everything",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_boxed_free_callback(gpointer boxed, gpointer user_data)
{
    GType *gtype = user_data;
    g_boxed_free(*gtype, boxed);
}

static void
rb_gi_arguments_convert_return_value_free_everything_glist_interface(
    ReturnValueToRubyData *data)
{
    GIInfoType type = data->metadata->element_type.interface_type;
    GType gtype = data->metadata->element_type.interface_gtype;
    switch (type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GList)[interface(%s)](%s) everything",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        break;
      case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            rb_raise(rb_eNotImpError,
                     "TODO: free GIArgument(GList)[interface(%s)](%s) everything",
                     g_info_type_to_string(type),
                     g_type_name(gtype));
        } else if (gtype == G_TYPE_VARIANT) {
            g_list_free_full(data->value->v_pointer,
                             (GDestroyNotify)g_variant_unref);
        } else {
            g_list_foreach(data->value->v_pointer,
                            rb_gi_boxed_free_callback,
                            &gtype);
            g_list_free(data->value->v_pointer);
        }
        break;
      case GI_INFO_TYPE_BOXED:
        g_list_foreach(data->value->v_pointer,
                       rb_gi_boxed_free_callback,
                       &gtype);
        g_list_free(data->value->v_pointer);
        break;
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GList)[interface(%s)](%s) everything",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        break;
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        g_list_free_full(data->value->v_pointer, g_object_unref);
        break;
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
                 "TODO: free GIArgument(GList)[interface(%s)](%s) everything",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_convert_return_value_free_everything_glist(
    ReturnValueToRubyData *data)
{
    if (!data->value->v_pointer)
        return;

    GITypeTag element_type_tag = data->metadata->element_type.tag;
    switch (element_type_tag) {
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
                 "TODO: free GIArgument(GList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        g_list_free_full(data->value->v_pointer, g_free);
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_convert_return_value_free_everything_glist_interface(data);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_convert_return_value_free_everything_gslist_interface(
    ReturnValueToRubyData *data)
{
    GIInfoType type = data->metadata->element_type.interface_type;
    GType gtype = data->metadata->element_type.interface_gtype;
    switch (type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GSList)[interface(%s)](%s) everything",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        break;
      case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            rb_raise(rb_eNotImpError,
                     "TODO: free GIArgument(GSList)[interface(%s)](%s) everything",
                     g_info_type_to_string(type),
                     g_type_name(gtype));
        } else {
            g_slist_foreach(data->value->v_pointer,
                            rb_gi_boxed_free_callback,
                            &gtype);
            g_slist_free(data->value->v_pointer);
        }
        break;
      case GI_INFO_TYPE_BOXED:
        g_slist_foreach(data->value->v_pointer,
                        rb_gi_boxed_free_callback,
                        &gtype);
        g_slist_free(data->value->v_pointer);
        break;
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GSList)[interface(%s)](%s) everything",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        break;
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        g_slist_free_full(data->value->v_pointer, g_object_unref);
        break;
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
                 "TODO: free GIArgument(GSList)[interface(%s)](%s) everything",
                 g_info_type_to_string(type),
                 g_type_name(gtype));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_convert_return_value_free_everything_gslist(
    ReturnValueToRubyData *data)
{
    if (!data->value->v_pointer)
        return;

    GITypeTag element_type_tag = data->metadata->element_type.tag;
    switch (element_type_tag) {
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
                 "TODO: free GIArgument(GSList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        g_slist_free_full(data->value->v_pointer, g_free);
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GSList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_convert_return_value_free_everything_gslist_interface(data);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GSList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_convert_return_value_free_everything(ReturnValueToRubyData *data)
{
    GITypeTag type_tag = data->metadata->type.tag;
    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
        break;
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
        /* We need to do nothing. */
        break;
      case GI_TYPE_TAG_GTYPE:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) everything",
                 g_type_tag_to_string(type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
        g_free(data->value->v_string);
        break;
      case GI_TYPE_TAG_FILENAME:
        g_free(data->value->v_string);
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_gi_arguments_convert_return_value_free_everything_array(data);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_convert_return_value_free_everything_interface(data);
        break;
      case GI_TYPE_TAG_GLIST:
        rb_gi_arguments_convert_return_value_free_everything_glist(data);
        break;
      case GI_TYPE_TAG_GSLIST:
        rb_gi_arguments_convert_return_value_free_everything_gslist(data);
        break;
      case GI_TYPE_TAG_GHASH:
        g_hash_table_unref(data->value->v_pointer);
        break;
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) everything",
                 g_type_tag_to_string(type_tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static VALUE
rb_gi_arguments_convert_return_value_ensure_body(VALUE user_data)
{
    ReturnValueToRubyData *data = (ReturnValueToRubyData *)user_data;

    switch (g_callable_info_get_caller_owns(data->args->info)) {
      case GI_TRANSFER_NOTHING:
        break;
      case GI_TRANSFER_CONTAINER:
        rb_gi_arguments_convert_return_value_free_container(data);
        break;
      case GI_TRANSFER_EVERYTHING:
        rb_gi_arguments_convert_return_value_free_everything(data);
        break;
      default:
        g_assert_not_reached();
        break;
    }

    return Qnil;
}

static VALUE
rb_gi_arguments_convert_return_value_ensure_ensure(VALUE user_data)
{
    ReturnValueToRubyData *data = (ReturnValueToRubyData *)user_data;
    rb_gi_arg_metadata_clear(data->metadata);
    return Qnil;
}

static VALUE
rb_gi_arguments_convert_return_value_ensure(VALUE user_data)
{
    return rb_ensure(rb_gi_arguments_convert_return_value_ensure_body,
                     user_data,
                     rb_gi_arguments_convert_return_value_ensure_ensure,
                     user_data);
}

VALUE
rb_gi_arguments_convert_return_value(RBGIArguments *args,
                                     GIArgument *return_value)
{
    if (g_callable_info_may_return_null(args->info) &&
        !return_value->v_pointer) {
        GITypeInfo return_value_info;
        g_callable_info_load_return_type(args->info, &return_value_info);
        GITypeTag return_value_tag = g_type_info_get_tag(&return_value_info);
        switch (return_value_tag) {
          case GI_TYPE_TAG_GLIST:
          case GI_TYPE_TAG_GSLIST:
            return rb_ary_new();
          default:
            return Qnil;
        }
    }

    ReturnValueToRubyData data;
    data.args = args;
    data.value = return_value;
    RBGIArgMetadata metadata;
    GITypeInfo *return_value_info = g_callable_info_get_return_type(args->info);
    rb_gi_arg_metadata_init_type_info(&metadata, return_value_info);
    rb_gi_arg_metadata_init_struct_info(&metadata, NULL, NULL);
    data.metadata = &metadata;

    return rb_ensure(rb_gi_arguments_convert_return_value_body, (VALUE)&data,
                     rb_gi_arguments_convert_return_value_ensure, (VALUE)&data);
}

void
rb_gi_arguments_fill_raw_out_gerror(RBGIArguments *args,
                                    VALUE rb_error)
{
    if (!g_callable_info_can_throw_gerror(args->info)) {
        return;
    }

    gint n_args = g_callable_info_get_n_args(args->info);
    /* GError ** isn't listed in args. */
    GError **gerror = *((gpointer *)(args->raw_args[n_args]));
    VALUE cGLibErrorInfo = rb_const_get(rbg_mGLib(), rb_intern("ErrorInfo"));
    if (NIL_P(rb_error)) {
        g_set_error(gerror,
                    RBG_RUBY_ERROR,
                    RBG_RUBY_ERROR_UNKNOWN,
                    "Unknown error");
    } else {
        VALUE message = rb_funcall(rb_error, rb_intern("message"), 0);
        VALUE backtrace = rb_funcall(rb_error, rb_intern("backtrace"), 0);
        VALUE formatted_backtrace =
            rb_ary_join(backtrace, rb_str_new_cstr("  \n"));
        GQuark gdomain = RBG_RUBY_ERROR;
        gint gcode = RBG_RUBY_ERROR_UNKNOWN;
        if (RVAL2CBOOL(rb_obj_is_kind_of(rb_error, cGLibErrorInfo))) {
            VALUE domain = rb_funcall(rb_error, rb_intern("domain"), 0);
            VALUE code = rb_funcall(rb_error, rb_intern("code"), 0);
            if (!NIL_P(domain) && !NIL_P(code)) {
                gdomain = g_quark_from_string(RVAL2CSTR(domain));
                gcode = NUM2INT(code);
            }
        }
        g_set_error(gerror,
                    gdomain,
                    gcode,
                    "%s\n  %s\n",
                    RVAL2CSTR(message),
                    RVAL2CSTR(formatted_backtrace));
    }
}

static void
rb_gi_arguments_fill_raw_result_interface(RBGIArguments *args,
                                          VALUE rb_result,
                                          gpointer raw_result,
                                          GITypeInfo *type_info,
                                          GITransfer transfer,
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
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw result(interface)[%s]: <%s>",
                 args->namespace,
                 args->name,
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_STRUCT:
      {
          gpointer value;
          GType gtype = g_registered_type_info_get_g_type(interface_info);
          if (gtype == G_TYPE_NONE) {
              /* Is it OK? */
              /* value = RVAL2GPTR(rb_result); */
              rb_raise(rb_eNotImpError,
                       "TODO: %s::%s: out raw result(interface)[%s][%s]: <%s>",
                       args->namespace,
                       args->name,
                       g_info_type_to_string(interface_type),
                       g_type_name(gtype),
                       g_base_info_get_name(interface_info));
          } else {
              value = RVAL2BOXED(rb_result, gtype);
              if (value && transfer == GI_TRANSFER_EVERYTHING) {
                  value = g_boxed_copy(gtype, value);
              }
          }
          if (is_return_value) {
              ffi_return_value->v_pointer = value;
          } else {
              *((gpointer *)raw_result) = value;
          }
      }
      break;
    case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw result(interface)[%s]: <%s>",
                 args->namespace,
                 args->name,
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_ENUM:
      {
          gint32 value;
          GType gtype = g_registered_type_info_get_g_type(interface_info);
          if (gtype == G_TYPE_NONE) {
              value = NUM2INT(rb_result);
          } else {
              value = RVAL2GENUM(rb_result, gtype);
          }
          if (is_return_value) {
              ffi_return_value->v_ulong = value;
          } else {
              *((gint *)raw_result) = value;
          }
      }
      break;
    case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw result(interface)[%s]: <%s>",
                 g_base_info_get_namespace(args->info),
                 g_base_info_get_name(args->info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
      {
          GObject *value = RVAL2GOBJ(rb_result);
          GType gtype = g_registered_type_info_get_g_type(interface_info);
          if (gtype != G_TYPE_NONE &&
              !G_TYPE_CHECK_INSTANCE_TYPE(value, gtype)) {
              rb_raise(rb_eArgError,
                       "%s::%s: must return <%s> object: <%" PRIsVALUE ">",
                       g_base_info_get_namespace(args->info),
                       g_base_info_get_name(args->info),
                       g_type_name(gtype),
                       rb_result);
          }
          if (transfer == GI_TRANSFER_EVERYTHING) {
              g_object_ref(value);
          }
          if (is_return_value) {
              ffi_return_value->v_pointer = value;
          } else {
              *((gpointer *)raw_result) = value;
          }
      }
      break;
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

static void
rb_gi_arguments_fill_raw_result_glist_interface(
    RBGIArguments *args,
    VALUE rb_result,
    gpointer raw_result,
    GITypeInfo *type_info,
    GITypeInfo *element_type_info,
    G_GNUC_UNUSED GITransfer transfer /* TODO */,
    gboolean is_return_value)
{
    GIFFIReturnValue *ffi_return_value = raw_result;
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    const gchar *interface_name;
    GType gtype;
    GList *list = NULL;

    interface_info = g_type_info_get_interface(element_type_info);
    interface_type = g_base_info_get_type(interface_info);
    interface_name = g_info_type_to_string(interface_type);
    gtype = g_registered_type_info_get_g_type(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
    case GI_INFO_TYPE_STRUCT:
    case GI_INFO_TYPE_BOXED:
    case GI_INFO_TYPE_ENUM:
    case GI_INFO_TYPE_FLAGS:
      g_base_info_unref(interface_info);
      g_base_info_unref(element_type_info);
      rb_raise(rb_eNotImpError,
               "TODO: %s::%s: out raw result(glist)[interface(%s)](%s)",
               g_base_info_get_namespace(args->info),
               g_base_info_get_name(args->info),
               interface_name,
               g_type_name(gtype));
      break;
    case GI_INFO_TYPE_OBJECT:
      list = RVAL2GOBJGLIST(rb_result);
      if (transfer == GI_TRANSFER_EVERYTHING) {
          g_list_foreach(list, (GFunc)g_object_ref, NULL);
      }
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
      g_base_info_unref(interface_info);
      g_base_info_unref(element_type_info);
      rb_raise(rb_eNotImpError,
               "TODO: %s::%s: out raw result(glist)[interface(%s)](%s)",
               g_base_info_get_namespace(args->info),
               g_base_info_get_name(args->info),
               interface_name,
               g_type_name(gtype));
      break;
    default:
      g_base_info_unref(interface_info);
      g_base_info_unref(element_type_info);
      g_assert_not_reached();
      break;
    }

    if (is_return_value) {
        ffi_return_value->v_ulong = (gulong)list;
    } else {
        *((gpointer *)raw_result) = list;
    }
}

static void
rb_gi_arguments_fill_raw_result_glist(RBGIArguments *args,
                                      VALUE rb_result,
                                      gpointer raw_result,
                                      GITypeInfo *type_info,
                                      GITransfer transfer,
                                      gboolean is_return_value)
{
    GIFFIReturnValue *ffi_return_value = raw_result;
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;

    element_type_info = g_type_info_get_param_type(type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);

    if (is_return_value) {
        ffi_return_value->v_ulong = (gulong)NULL;
    } else {
        *((gpointer *)raw_result) = NULL;
    }

    switch (element_type_tag) {
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
      g_base_info_unref(element_type_info);
      rb_raise(rb_eNotImpError,
               "TODO: %s::%s: out raw result(GList)[%s]",
               g_base_info_get_namespace(args->info),
               g_base_info_get_name(args->info),
               g_type_tag_to_string(element_type_tag));
      break;
    case GI_TYPE_TAG_INTERFACE:
      rb_gi_arguments_fill_raw_result_glist_interface(
          args,
          rb_result,
          raw_result,
          type_info,
          element_type_info,
          transfer,
          is_return_value);
      break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
      g_base_info_unref(element_type_info);
      rb_raise(rb_eNotImpError,
               "TODO: %s::%s: out raw result(GList)[%s]",
               g_base_info_get_namespace(args->info),
               g_base_info_get_name(args->info),
               g_type_tag_to_string(element_type_tag));
      break;
    default:
      g_base_info_unref(element_type_info);
      g_assert_not_reached();
      break;
    }
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
                                gboolean may_return_null,
                                gboolean is_return_value)
{
    GIFFIReturnValue *ffi_return_value = raw_result;

    if (may_return_null && RB_NIL_P(rb_result)) {
        if (is_return_value) {
            ffi_return_value->v_pointer = NULL;
        } else {
            *((gpointer *)raw_result) = NULL;
        }
        return;
    }

    GITypeTag type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
        g_assert_not_reached();
        break;
      case GI_TYPE_TAG_BOOLEAN:
        if (is_return_value) {
            ffi_return_value->v_ulong = RVAL2CBOOL(rb_result);
        } else {
            *((gboolean *)raw_result) = RVAL2CBOOL(rb_result);
        }
        break;
      case GI_TYPE_TAG_INT8:
        if (is_return_value) {
            ffi_return_value->v_long = NUM2CHR(rb_result);
        } else {
            *((gint8 *)raw_result) = NUM2CHR(rb_result);
        }
        break;
      case GI_TYPE_TAG_UINT8:
        if (is_return_value) {
            ffi_return_value->v_ulong = (guint8)NUM2CHR(rb_result);
        } else {
            *((guint8 *)raw_result) = (guint8)NUM2CHR(rb_result);
        }
        break;
      case GI_TYPE_TAG_INT16:
        if (is_return_value) {
            ffi_return_value->v_long = NUM2SHORT(rb_result);
        } else {
            *((gint16 *)raw_result) = NUM2SHORT(rb_result);
        }
        break;
      case GI_TYPE_TAG_UINT16:
        if (is_return_value) {
            ffi_return_value->v_ulong = NUM2USHORT(rb_result);
        } else {
            *((guint16 *)raw_result) = NUM2USHORT(rb_result);
        }
        break;
      case GI_TYPE_TAG_INT32:
        if (is_return_value) {
            ffi_return_value->v_long = NUM2INT(rb_result);
        } else {
            *((gint32 *)raw_result) = NUM2INT(rb_result);
        }
        break;
      case GI_TYPE_TAG_UINT32:
        if (is_return_value) {
            ffi_return_value->v_ulong = NUM2UINT(rb_result);
        } else {
            *((guint32 *)raw_result) = NUM2UINT(rb_result);
        }
        break;
      case GI_TYPE_TAG_INT64:
        *((gint64 *)raw_result) = NUM2LL(rb_result);
        break;
      case GI_TYPE_TAG_UINT64:
        *((guint64 *)raw_result) = NUM2ULL(rb_result);
        break;
      case GI_TYPE_TAG_FLOAT:
        *((gfloat *)raw_result) = NUM2DBL(rb_result);
        break;
      case GI_TYPE_TAG_DOUBLE:
        *((gdouble *)raw_result) = NUM2DBL(rb_result);
        break;
      case GI_TYPE_TAG_GTYPE:
        if (is_return_value) {
            ffi_return_value->v_ulong = rbgobj_gtype_from_ruby(rb_result);
        } else {
            *((gsize *)raw_result) = rbgobj_gtype_from_ruby(rb_result);
        }
        break;
      case GI_TYPE_TAG_UTF8:
        {
            gchar *result = (gchar *)RVAL2CSTR_ACCEPT_SYMBOL(rb_result);
            if (transfer == GI_TRANSFER_EVERYTHING) {
                result = g_strdup(result);
            }
            if (is_return_value) {
                ffi_return_value->v_ulong = (gulong)result;
            } else {
                *((gchar **)raw_result) = (gchar *)result;
            }
        }
        break;
      case GI_TYPE_TAG_FILENAME:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw result(%s)",
                 args->namespace,
                 args->name,
                 g_type_tag_to_string(type_tag));
        /* if (is_return_value) { */
        /*     ffi_return_value->v_ulong = */
        /*         (gulong)rbg_filename_from_ruby(rb_result); */
        /* } else { */
        /*     *((gchar **)raw_result) = */
        /*         (gchar *)rbg_filename_from_ruby(rb_result); */
        /* } */
        /* free */
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw result(%s)",
                 args->namespace,
                 args->name,
                 g_type_tag_to_string(type_tag));
        /* if (is_return_value) { */
        /*     ffi_return_value->v_ulong = (gulong)(argument.v_pointer); */
        /* } else { */
        /*     *((gpointer *)raw_result) = argument.v_pointer; */
        /* } */
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_fill_raw_result_interface(args,
                                                  rb_result,
                                                  raw_result,
                                                  type_info,
                                                  transfer,
                                                  is_return_value);
        break;
      case GI_TYPE_TAG_GLIST:
        rb_gi_arguments_fill_raw_result_glist(args,
                                              rb_result,
                                              raw_result,
                                              type_info,
                                              transfer,
                                              is_return_value);
        break;
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw result(%s)",
                 args->namespace,
                 args->name,
                 g_type_tag_to_string(type_tag));
        /* if (is_return_value) { */
        /*     ffi_return_value->v_ulong = (gulong)(argument.v_pointer); */
        /* } else { */
        /*     *((gpointer *)raw_result) = argument.v_pointer; */
        /* } */
        break;
      case GI_TYPE_TAG_ERROR:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw result(%s)",
                 args->namespace,
                 args->name,
                 g_type_tag_to_string(type_tag));
        /* if (is_return_value) { */
        /*     ffi_return_value->v_ulong = (gulong)(argument.v_pointer); */
        /* } else { */
        /*     *((GError **)raw_result) = argument.v_pointer; */
        /* } */
        break;
      case GI_TYPE_TAG_UNICHAR:
        if (is_return_value) {
            ffi_return_value->v_ulong = NUM2UINT(rb_result);
        } else {
            *((gunichar *)raw_result) = NUM2UINT(rb_result);
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
    const gboolean rb_results_is_array = RB_TYPE_P(rb_results, RUBY_T_ARRAY);
    int i_rb_result = 0;
    guint i;
    GITypeInfo *return_type_info;
    GITypeTag return_type_tag;

    return_type_info = g_callable_info_get_return_type(args->info);
    return_type_tag = g_type_info_get_tag(return_type_info);
    if (return_type_tag != GI_TYPE_TAG_VOID) {
        GITransfer transfer = g_callable_info_get_caller_owns(args->info);
        gboolean may_return_null = g_callable_info_may_return_null(args->info);
        if (args->out_args->len == 0) {
            VALUE rb_return_value = rb_results;
            rb_gi_arguments_fill_raw_result(args,
                                            rb_return_value,
                                            raw_return_value,
                                            return_type_info,
                                            transfer,
                                            may_return_null,
                                            TRUE);
        } else {
            VALUE rb_return_value;
            if (rb_results_is_array) {
                rb_return_value = RARRAY_AREF(rb_results, i_rb_result);
            } else {
                rb_return_value = rb_results;
            }
            i_rb_result++;
            rb_gi_arguments_fill_raw_result(args,
                                            rb_return_value,
                                            raw_return_value,
                                            return_type_info,
                                            transfer,
                                            may_return_null,
                                            TRUE);
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
        VALUE rb_result_value;
        if (rb_results_is_array) {
            rb_result_value = RARRAY_AREF(rb_results, i_rb_result);
        } else {
            if (i_rb_result == 0) {
                rb_result_value = rb_results;
            } else {
                rb_result_value = RUBY_Qnil;
            }
        }
        i_rb_result++;
        rb_gi_arguments_fill_raw_result(args,
                                        rb_result_value,
                                        argument->v_pointer,
                                        type_info,
                                        transfer,
                                        metadata->may_be_null_p,
                                        FALSE);
        g_base_info_unref(type_info);
    }
}
