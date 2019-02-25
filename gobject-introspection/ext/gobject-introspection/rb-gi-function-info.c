/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2019  Ruby-GNOME2 Project Team
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

#ifdef HAVE_RUBY_THREAD_H
#  include <ruby/thread.h>
#  define RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_TYPE void *
#  define RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_VALUE NULL
#else
#  define rb_thread_call_without_gvl(func, func_data, ubf, ubf_data) \
    rb_thread_blocking_region(func, func_data, ubf, ubf_data)
#  define RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_TYPE VALUE
#  define RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_VALUE Qnil
#endif

#define RG_TARGET_NAMESPACE rb_cGIFunctionInfo
#define SELF(self) RVAL2GI_FUNCTION_INFO(self)

typedef struct _RBGICallback {
    GIArgInfo *arg_info;
    GITypeInfo *type_info;
    GICallbackInfo *callback_info;
    ffi_cif cif;
    ffi_closure *closure;
} RBGICallback;

struct RBGICallbackData_ {
    RBGICallback *callback;
    RBGIArgMetadata *metadata;
    VALUE rb_callback;
    GObject *owner;
    VALUE rb_owner;
};

static VALUE RG_TARGET_NAMESPACE;
static VALUE rb_cGLibError;
static const char *callbacks_key = "gi_callbacks";
static GPtrArray *callback_finders;

GType
gi_function_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
        type = g_boxed_type_register_static("GIFunctionInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_set_unlock_gvl(VALUE self, VALUE rb_boolean)
{
    return rb_iv_set(self, "unlock_gvl", rb_boolean);
}

static VALUE
rg_unlock_gvl_p(VALUE self)
{
    if (!RVAL2CBOOL(rb_ivar_defined(self, rb_intern("unlock_gvl")))) {
        rb_iv_set(self, "unlock_gvl", Qfalse);
    }
    return rb_iv_get(self, "unlock_gvl");
}

static VALUE
rg_symbol(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_function_info_get_symbol(info));
}

static VALUE
rg_flags(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return GI_FUNCTION_INFO_FLAGS2RVAL(g_function_info_get_flags(info));
}

static VALUE
rg_property(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL(g_function_info_get_property(info));
}

static VALUE
rg_vfunc(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL(g_function_info_get_vfunc(info));
}

static void
allocate_arguments(GICallableInfo *info,
                   GArray *in_args, GArray *out_args,
                   GPtrArray *args_metadata)
{
    gint i, n_args;

    n_args = g_callable_info_get_n_args(info);
    for (i = 0; i < n_args; i++) {
        GIArgument argument;
        RBGIArgMetadata *metadata;
        GIArgInfo *arg_info;
        GIDirection direction;

        memset(&argument, 0, sizeof(GIArgument));

        metadata = ALLOC(RBGIArgMetadata);
        metadata->callable_info = info;
        arg_info = &(metadata->arg_info);
        g_callable_info_load_arg(info, i, arg_info);
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
            metadata->in_arg_index = in_args->len;
            g_array_append_val(in_args, argument);
        }
        if (direction == GI_DIRECTION_OUT || direction == GI_DIRECTION_INOUT) {
            metadata->out_arg_index = out_args->len;
            g_array_append_val(out_args, argument);
        }

        g_ptr_array_add(args_metadata, metadata);
    }
}

static void
fill_metadata_callback(GPtrArray *args_metadata)
{
    guint i;

    for (i = 0; i < args_metadata->len; i++) {
        RBGIArgMetadata *metadata;
        GIArgInfo *arg_info;
        gint closure_index;
        gint destroy_index;

        metadata = g_ptr_array_index(args_metadata, i);

        arg_info = &(metadata->arg_info);
        closure_index = g_arg_info_get_closure(arg_info);
        if (closure_index != -1) {
            RBGIArgMetadata *closure_metadata;
            closure_metadata = g_ptr_array_index(args_metadata, closure_index);
            closure_metadata->closure_p = TRUE;
            metadata->closure_in_arg_index = closure_metadata->in_arg_index;
        }

        destroy_index = g_arg_info_get_destroy(arg_info);
        if (destroy_index != -1) {
            RBGIArgMetadata *destroy_metadata;
            destroy_metadata = g_ptr_array_index(args_metadata, destroy_index);
            destroy_metadata->destroy_p = TRUE;
            metadata->destroy_in_arg_index = destroy_metadata->in_arg_index;
        }
    }
}

static void
fill_metadata_array(GPtrArray *args_metadata)
{
    guint i;

    for (i = 0; i < args_metadata->len; i++) {
        RBGIArgMetadata *metadata;
        RBGIArgMetadata *array_length_metadata;
        GIArgInfo *arg_info;
        GITypeInfo type_info;
        gint array_length_index = -1;

        metadata = g_ptr_array_index(args_metadata, i);
        arg_info = &(metadata->arg_info);

        g_arg_info_load_type(arg_info, &type_info);
        if (g_type_info_get_tag(&type_info) != GI_TYPE_TAG_ARRAY) {
            continue;
        }
        metadata->array_p = TRUE;

        array_length_index = g_type_info_get_array_length(&type_info);
        if (array_length_index == -1) {
            continue;
        }

        array_length_metadata = g_ptr_array_index(args_metadata,
                                                  array_length_index);
        array_length_metadata->array_length_p = TRUE;
        array_length_metadata->rb_arg_index = -1;
        array_length_metadata->array_in_arg_index =
            metadata->in_arg_index;
        metadata->array_length_in_arg_index =
            array_length_metadata->in_arg_index;
        metadata->array_length_arg_index = array_length_index;
    }
}

static void
fill_metadata_array_from_callable_info(GPtrArray *args_metadata,
                                       GICallableInfo *info)
{
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

    array_length_metadata = g_ptr_array_index(args_metadata, array_length_index);
    array_length_metadata->array_length_p = TRUE;
    array_length_metadata->rb_arg_index = -1;
}

static void
fill_metadata_rb_arg_index(GPtrArray *args_metadata)
{
    guint i;
    gint rb_arg_index = 0;

    for (i = 0; i < args_metadata->len; i++) {
        RBGIArgMetadata *metadata;

        metadata = g_ptr_array_index(args_metadata, i);

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
fill_metadata(GPtrArray *args_metadata, GICallableInfo *info)
{
    fill_metadata_callback(args_metadata);
    fill_metadata_array(args_metadata);
    fill_metadata_array_from_callable_info(args_metadata, info);
    fill_metadata_rb_arg_index(args_metadata);
}

static void
rb_gi_callback_free(RBGICallback *callback)
{
    g_callable_info_free_closure(callback->callback_info,
                                 callback->closure);
    g_base_info_unref(callback->callback_info);
    g_base_info_unref(callback->type_info);
    xfree(callback);
}

static void
rb_gi_callback_data_weak_notify(gpointer data,
                                G_GNUC_UNUSED GObject *where_the_object_was)
{
    RBGICallbackData *callback_data = data;
    callback_data->owner = NULL;
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

static void
rb_gi_callback_data_destroy_notify(gpointer data)
{
    RBGICallbackData *callback_data = data;
    rb_gi_callback_data_free(callback_data);
}

static gpointer
find_callback_function(GIArgInfo *info)
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

void
rb_gi_callback_register_finder(RBGICallbackFinderFunc finder)
{
    g_ptr_array_add(callback_finders, finder);
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

static void arguments_init(GArray **in_args,
                           GArray **out_args,
                           GPtrArray **args_metadata);
static void arguments_free(VALUE rb_arguments,
                           GArray *in_args,
                           GArray *out_args,
                           GPtrArray *args_metadata);

static void
argument_from_raw_data_interface(GICallableInfo *callable_info,
                                 void *raw_arg,
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
                 "TODO: %s::%s: raw data -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(callable_info),
                 g_base_info_get_name(callable_info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_STRUCT:
      argument->v_pointer = *((gpointer *)(raw_arg));
      break;
    case GI_INFO_TYPE_BOXED:
    case GI_INFO_TYPE_ENUM:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw data -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(callable_info),
                 g_base_info_get_name(callable_info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_FLAGS:
      argument->v_int32= *((gint32 *)(raw_arg));
      break;
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
      argument->v_pointer = *((gpointer *)(raw_arg));
      break;
    case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: raw data -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(callable_info),
                 g_base_info_get_name(callable_info),
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
                 "TODO: %s::%s: raw data -> GIArgument(interface)[%s]: <%s>",
                 g_base_info_get_namespace(callable_info),
                 g_base_info_get_name(callable_info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    }

    g_base_info_unref(interface_info);
}

static void
argument_from_raw_data(GICallableInfo *callable_info,
                       void **raw_args,
                       GArray *in_args,
                       GArray *out_args,
                       GPtrArray *args_metadata,
                       guint i)
{
    RBGIArgMetadata *metadata;
    GIArgument *argument;
    GITypeInfo *type_info;
    GITypeTag type_tag;

    metadata = g_ptr_array_index(args_metadata, i);

    if (metadata->direction == GI_DIRECTION_INOUT) {
        argument = &g_array_index(in_args, GIArgument, metadata->in_arg_index);
        argument->v_pointer = *((gpointer *)(raw_args[i]));
        return;
    } else if (metadata->direction == GI_DIRECTION_OUT) {
        argument = &g_array_index(out_args, GIArgument, metadata->out_arg_index);
        argument->v_pointer = *((gpointer *)(raw_args[i]));
        return;
    }

    argument = &g_array_index(in_args, GIArgument, metadata->in_arg_index);
    type_info = g_arg_info_get_type(&(metadata->arg_info));
    type_tag = g_type_info_get_tag(type_info);

    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
        argument->v_pointer = *((gpointer *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_BOOLEAN:
        argument->v_boolean = *((gboolean *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_INT8:
        argument->v_int8 = *((gint8 *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_UINT8:
        argument->v_uint8 = *((guint8 *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_INT16:
        argument->v_int16 = *((gint16 *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_UINT16:
        argument->v_uint16 = *((guint16 *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_INT32:
        argument->v_int32 = *((gint32 *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_UINT32:
        argument->v_uint32 = *((guint32 *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_INT64:
        argument->v_int64 = *((gint64 *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_UINT64:
        argument->v_uint64 = *((guint64 *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_FLOAT:
        argument->v_float = *((gfloat *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_DOUBLE:
        argument->v_double = *((gdouble *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_GTYPE:
        argument->v_size = *((gsize *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        argument->v_string = *((gchar **)(raw_args[i]));
        break;
      case GI_TYPE_TAG_ARRAY:
        argument->v_pointer = *((gpointer *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_INTERFACE:
        argument_from_raw_data_interface(callable_info,
                                         raw_args[i],
                                         argument,
                                         type_info);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        argument->v_pointer = *((gpointer *)(raw_args[i]));
        break;
      case GI_TYPE_TAG_ERROR:
        argument->v_pointer = *((GError **)(raw_args[i]));
        break;
      case GI_TYPE_TAG_UNICHAR:
        argument->v_uint32 = *((gunichar *)(raw_args[i]));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    g_base_info_unref(type_info);
}

static void
arguments_from_raw_data(GICallableInfo *callable_info,
                        void **args,
                        GArray *in_args,
                        GArray *out_args,
                        GPtrArray *args_metadata)
{
    guint i;

    for (i = 0; i < args_metadata->len; i++) {
        argument_from_raw_data(callable_info,
                               args,
                               in_args,
                               out_args,
                               args_metadata,
                               i);
    }
}

static void
in_arguments_to_ruby(GArray *in_args,
                     GArray *out_args,
                     GPtrArray *args_metadata,
                     GArray *rb_args)
{
    guint i;

    for (i = 0; i < args_metadata->len; i++) {
        RBGIArgMetadata *metadata;
        GIArgument *argument;
        GITypeInfo *type_info;
        VALUE rb_arg;

        metadata = g_ptr_array_index(args_metadata, i);

        if (metadata->direction == GI_DIRECTION_OUT) {
            continue;
        }
        if (metadata->closure_p) {
            continue;
        }

        argument = &g_array_index(in_args, GIArgument, metadata->in_arg_index);
        type_info = g_arg_info_get_type(&(metadata->arg_info));
        rb_arg = GI_ARGUMENT2RVAL(argument,
                                  FALSE,
                                  type_info,
                                  in_args,
                                  out_args,
                                  args_metadata);
        g_array_append_val(rb_args, rb_arg);
    }
}

static void
out_argument_to_raw_data_interface(GICallableInfo *callable_info,
                                   GIArgument *argument,
                                   gpointer result,
                                   GITypeInfo *type_info,
                                   G_GNUC_UNUSED GITransfer transfer /* TODO */,
                                   gboolean is_return_value)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GIFFIReturnValue *ffi_return_value = result;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
    case GI_INFO_TYPE_STRUCT:
    case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw data(interface)[%s]: <%s>",
                 g_base_info_get_namespace(callable_info),
                 g_base_info_get_name(callable_info),
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_ENUM:
      if (is_return_value) {
          ffi_return_value->v_ulong = argument->v_int;
      } else {
          *((gint *)result) = argument->v_int;
      }
      break;
    case GI_INFO_TYPE_FLAGS:
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
    case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: %s::%s: out raw data(interface)[%s]: <%s>",
                 g_base_info_get_namespace(callable_info),
                 g_base_info_get_name(callable_info),
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
                 "TODO: %s::%s: out raw data(interface)[%s]: <%s>",
                 g_base_info_get_namespace(callable_info),
                 g_base_info_get_name(callable_info),
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
    * https://github.com/ruby-gnome2/ruby-gnome2/issues/758#issuecomment-243149237
    * https://github.com/libffi/libffi/pull/216

  This ffi_return_value case implementation is based on
  gi_type_info_extract_ffi_return_value().
*/
static void
out_argument_to_raw_data(GICallableInfo *callable_info,
                         VALUE rb_result,
                         gpointer result,
                         GITypeInfo *type_info,
                         GITransfer transfer,
                         gboolean is_return_value)
{
    GIArgument argument;
    GITypeTag type_tag;
    GIFFIReturnValue *ffi_return_value = result;

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
            *((gboolean *)result) = argument.v_boolean;
        }
        break;
      case GI_TYPE_TAG_INT8:
        if (is_return_value) {
            ffi_return_value->v_long = argument.v_int8;
        } else {
            *((gint8 *)result) = argument.v_int8;
        }
        break;
      case GI_TYPE_TAG_UINT8:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_uint8;
        } else {
            *((guint8 *)result) = argument.v_uint8;
        }
        break;
      case GI_TYPE_TAG_INT16:
        if (is_return_value) {
            ffi_return_value->v_long = argument.v_int16;
        } else {
            *((gint16 *)result) = argument.v_int16;
        }
        break;
      case GI_TYPE_TAG_UINT16:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_uint16;
        } else {
            *((guint16 *)result) = argument.v_uint16;
        }
        break;
      case GI_TYPE_TAG_INT32:
        if (is_return_value) {
            ffi_return_value->v_long = argument.v_int32;
        } else {
            *((gint32 *)result) = argument.v_int32;
        }
        break;
      case GI_TYPE_TAG_UINT32:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_uint32;
        } else {
            *((guint32 *)result) = argument.v_uint32;
        }
        break;
      case GI_TYPE_TAG_INT64:
        *((gint64 *)result) = argument.v_int64;
        break;
      case GI_TYPE_TAG_UINT64:
        *((guint64 *)result) = argument.v_uint64;
        break;
      case GI_TYPE_TAG_FLOAT:
        *((gfloat *)result) = argument.v_float;
        break;
      case GI_TYPE_TAG_DOUBLE:
        *((gdouble *)result) = argument.v_double;
        break;
      case GI_TYPE_TAG_GTYPE:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_size;
        } else {
            *((gsize *)result) = argument.v_size;
        }
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        if (is_return_value) {
            ffi_return_value->v_ulong = (gulong)(argument.v_string);
        } else {
            *((gchar **)result) = argument.v_string;
        }
        break;
      case GI_TYPE_TAG_ARRAY:
        if (is_return_value) {
            ffi_return_value->v_ulong = (gulong)(argument.v_pointer);
        } else {
            *((gpointer *)result) = argument.v_pointer;
        }
        break;
      case GI_TYPE_TAG_INTERFACE:
        out_argument_to_raw_data_interface(callable_info,
                                           &argument,
                                           result,
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
            *((gpointer *)result) = argument.v_pointer;
        }
        break;
      case GI_TYPE_TAG_ERROR:
        if (is_return_value) {
            ffi_return_value->v_ulong = (gulong)(argument.v_pointer);
        } else {
            *((GError **)result) = argument.v_pointer;
        }
        break;
      case GI_TYPE_TAG_UNICHAR:
        if (is_return_value) {
            ffi_return_value->v_ulong = argument.v_uint32;
        } else {
            *((gunichar *)result) = argument.v_uint32;
        }
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
out_arguments_to_raw_data(GICallableInfo *callable_info,
                          VALUE rb_results,
                          void *result,
                          GArray *out_args,
                          GPtrArray *args_metadata)
{
    int i_rb_result = 0;
    guint i;
    GITypeInfo *return_type_info;
    GITypeTag return_type_tag;

    return_type_info = g_callable_info_get_return_type(callable_info);
    return_type_tag = g_type_info_get_tag(return_type_info);
    if (return_type_tag != GI_TYPE_TAG_VOID) {
        GITransfer transfer;
        transfer = g_callable_info_get_caller_owns(callable_info);
        if (out_args->len == 0) {
            VALUE rb_return_value = rb_results;
            out_argument_to_raw_data(callable_info,
                                     rb_return_value,
                                     result,
                                     return_type_info,
                                     transfer,
                                     TRUE);
        } else {
            out_argument_to_raw_data(callable_info,
                                     RARRAY_AREF(rb_results, i_rb_result),
                                     result,
                                     return_type_info,
                                     transfer,
                                     TRUE);
            i_rb_result++;
        }
    }
    g_base_info_unref(return_type_info);

    for (i = 0; i < args_metadata->len; i++) {
        RBGIArgMetadata *metadata;
        GIArgument *argument;
        GITypeInfo *type_info;
        GITransfer transfer;

        metadata = g_ptr_array_index(args_metadata, i);

        /* TODO: support GI_DIRECTION_INOUT */
        if (metadata->direction != GI_DIRECTION_OUT) {
            continue;
        }

        argument = &g_array_index(out_args, GIArgument, metadata->out_arg_index);
        type_info = g_arg_info_get_type(&(metadata->arg_info));
        transfer = g_arg_info_get_ownership_transfer(&(metadata->arg_info));
        out_argument_to_raw_data(callable_info,
                                 RARRAY_AREF(rb_results, i_rb_result),
                                 argument->v_pointer,
                                 type_info,
                                 transfer,
                                 FALSE);
        i_rb_result++;
        g_base_info_unref(type_info);
    }
}

static void
ffi_closure_callback(G_GNUC_UNUSED ffi_cif *cif,
                     void *result,
                     void **raw_args,
                     void *data)
{
    RBGICallback *callback = data;
    RBGICallbackData *callback_data = NULL;
    GArray *in_args;
    GArray *out_args;
    GPtrArray *args_metadata;
    VALUE rb_results;

    arguments_init(&in_args, &out_args, &args_metadata);
    allocate_arguments(callback->callback_info,
                       in_args,
                       out_args,
                       args_metadata);
    fill_metadata(args_metadata, callback->callback_info);
    arguments_from_raw_data(callback->callback_info,
                            raw_args,
                            in_args,
                            out_args,
                            args_metadata);

    {
        guint i;

        for (i = 0; i < args_metadata->len; i++) {
            RBGIArgMetadata *metadata;

            metadata = g_ptr_array_index(args_metadata, i);
            if (!metadata->closure_p) {
                continue;
            }

            callback_data = *((RBGICallbackData **)(raw_args[i]));
            break;
        }

        if (!callback_data && args_metadata->len > 0) {
            RBGIArgMetadata *metadata;
            GIArgInfo *arg_info;
            GITypeInfo *type_info;
            GITypeTag type_tag;
            gboolean is_pointer;
            const gchar *arg_name;

            i = args_metadata->len - 1;
            metadata = g_ptr_array_index(args_metadata, i);
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
        GArray *rb_args;

        rb_args = g_array_new(FALSE, FALSE, sizeof(VALUE));
        in_arguments_to_ruby(in_args,
                             out_args,
                             args_metadata,
                             rb_args);
        CONST_ID(id_call, "call");
        /* TODO: use rb_protect() */
        rb_results = rb_funcallv(callback_data->rb_callback,
                                 id_call,
                                 rb_args->len,
                                 (VALUE *)(void *)(rb_args->data));
        g_array_free(rb_args, TRUE);
    }

    out_arguments_to_raw_data(callback->callback_info,
                              rb_results,
                              result,
                              out_args,
                              args_metadata);

    if (callback_data->metadata->scope_type == GI_SCOPE_TYPE_ASYNC) {
        rb_gi_callback_data_free(callback_data);
    }
}

static void
in_callback_argument_from_ruby(RBGIArgMetadata *metadata,
                               GArray *in_args,
                               VALUE self,
                               VALUE rb_arguments)
{
    gpointer callback_function;
    GIArgInfo *arg_info;
    GIArgument *callback_argument;
    GIArgument *closure_argument = NULL;
    GIArgument *destroy_argument = NULL;
    RBGICallback *callback = NULL;

    arg_info = &(metadata->arg_info);

    callback_argument = &(g_array_index(in_args,
                                        GIArgument,
                                        metadata->in_arg_index));
    if (metadata->closure_in_arg_index != -1) {
        closure_argument = &(g_array_index(in_args,
                                           GIArgument,
                                           metadata->closure_in_arg_index));
    }
    if (metadata->destroy_in_arg_index != -1) {
        destroy_argument = &(g_array_index(in_args,
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

    callback_function = find_callback_function(arg_info);
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
        RBGICallbackData *callback_data;
        VALUE rb_owner = self;
        static VALUE mGLibObject = Qnil;

        if (rb_gi_is_debug_mode()) {
            GICallableInfo *callable_info = metadata->callable_info;
            g_print("[rb-gi] callback: %s::%s()\n",
                    g_base_info_get_namespace(callable_info),
                    g_base_info_get_name(callable_info));
            rb_p(rb_ary_new_from_args(2, self, rb_arguments));
        }

        callback_data = ALLOC(RBGICallbackData);
        callback_data->callback = callback;
        callback_data->metadata = metadata;
        callback_data->rb_callback = rb_block_proc();
        if (NIL_P(mGLibObject)) {
            mGLibObject = rb_const_get(mGLib, rb_intern("Object"));
        }
        if (NIL_P(rb_owner)) {
            /* Module function case. */
            VALUE rb_first_argument = rb_ary_entry(rb_arguments, 0);
            if (rb_obj_is_kind_of(rb_first_argument, mGLibObject)) {
                /* If the first argument of the module function call is
                   GObject, it's suitable for owner.
                   For example: pango_cairo_context_set_shape_renderer() */
                rb_owner = rb_first_argument;
            }
        }
        if (rb_obj_is_kind_of(rb_owner, mGLibObject)) {
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
                static VALUE rb_mGI = Qnil;
                if (NIL_P(rb_mGI)) {
                    rb_mGI = rb_const_get(rb_cObject,
                                          rb_intern("GObjectIntrospection"));
                }
                rbgobj_add_relative(rb_mGI, callback_data->rb_callback);
                callback_data->rb_owner = rb_mGI;
            } else {
                /* Class method case. */
                rbgobj_add_relative(rb_owner, callback_data->rb_callback);
                callback_data->rb_owner = rb_owner;
            }
            callback_data->owner = NULL;
        }
        closure_argument->v_pointer = callback_data;
    }

    if (destroy_argument) {
        destroy_argument->v_pointer = rb_gi_callback_data_destroy_notify;
    }
}

static void
in_argument_from_ruby(GICallableInfo *callable_info,
                      RBGIArgMetadata *metadata,
                      VALUE rb_arguments,
                      GArray *in_args,
                      GPtrArray *args_metadata,
                      VALUE self)
{
    if (metadata->callback_p && !metadata->destroy_p) {
        in_callback_argument_from_ruby(metadata, in_args, self, rb_arguments);
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

        if (RARRAY_LEN(rb_arguments) > metadata->rb_arg_index) {
            rb_argument = RARRAY_PTR(rb_arguments)[metadata->rb_arg_index];
        }
        array_argument = &(g_array_index(in_args,
                                         GIArgument,
                                         metadata->in_arg_index));
        if (metadata->array_length_in_arg_index != -1) {
            RBGIArgMetadata *length_metadata;
            length_argument =
                &(g_array_index(in_args,
                                GIArgument,
                                metadata->array_length_in_arg_index));
            length_metadata =
                g_ptr_array_index(args_metadata,
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

        if (RARRAY_LEN(rb_arguments) > metadata->rb_arg_index) {
            rb_argument = RARRAY_PTR(rb_arguments)[metadata->rb_arg_index];
        }
        argument = &(g_array_index(in_args, GIArgument, metadata->in_arg_index));
        rb_gi_in_argument_from_ruby(callable_info,
                                    argument,
                                    &(metadata->arg_info),
                                    metadata->rb_arg_index,
                                    rb_argument,
                                    self);
    }
}

static void
out_argument_from_ruby(RBGIArgMetadata *metadata, GArray *out_args)
{
    GIArgument *argument;

    argument = &(g_array_index(out_args, GIArgument, metadata->out_arg_index));
    rb_gi_out_argument_init(argument, &(metadata->arg_info));
}

static void
arg_metadata_free(gpointer data)
{
    RBGIArgMetadata *metadata = data;
    if (metadata->scope_type == GI_SCOPE_TYPE_ASYNC ||
        metadata->scope_type == GI_SCOPE_TYPE_NOTIFIED) {
        return;
    }
    xfree(metadata);
}

static void
arguments_from_ruby(GICallableInfo *info, VALUE self, VALUE rb_arguments,
                    GArray *in_args, GArray *out_args,
                    GPtrArray *args_metadata)
{
    gint i, n_args;

    allocate_arguments(info, in_args, out_args, args_metadata);
    fill_metadata(args_metadata, info);

    n_args = g_callable_info_get_n_args(info);
    for (i = 0; i < n_args; i++) {
        RBGIArgMetadata *metadata;

        metadata = g_ptr_array_index(args_metadata, i);
        if (metadata->in_arg_index != -1) {
            in_argument_from_ruby(info,
                                  metadata,
                                  rb_arguments,
                                  in_args,
                                  args_metadata,
                                  self);
        } else {
            out_argument_from_ruby(metadata, out_args);
        }
    }
}

static VALUE
out_arguments_to_ruby(GICallableInfo *callable_info,
                      GArray *in_args, GArray *out_args,
                      GPtrArray *args_metadata)
{
    gint i, n_args;
    VALUE rb_out_args;

    rb_out_args = rb_ary_new();
    n_args = g_callable_info_get_n_args(callable_info);
    for (i = 0; i < n_args; i++) {
        RBGIArgMetadata *metadata;
        GIArgument *argument = NULL;
        VALUE rb_argument;

        metadata = g_ptr_array_index(args_metadata, i);
        if (metadata->array_length_p) {
            continue;
        }

        switch (metadata->direction) {
          case GI_DIRECTION_IN:
            break;
          case GI_DIRECTION_OUT:
            argument = &g_array_index(out_args, GIArgument,
                                      metadata->out_arg_index);
            break;
          case GI_DIRECTION_INOUT:
            argument = &g_array_index(in_args, GIArgument,
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
                                           in_args,
                                           out_args,
                                           args_metadata);
        rb_ary_push(rb_out_args, rb_argument);
    }

    if (RARRAY_LEN(rb_out_args) == 0) {
        return Qnil;
    } else {
        return rb_out_args;
    }
}

static void
arguments_init(GArray **in_args, GArray **out_args, GPtrArray **args_metadata)
{
    *in_args = g_array_new(FALSE, FALSE, sizeof(GIArgument));
    *out_args = g_array_new(FALSE, FALSE, sizeof(GIArgument));
    *args_metadata = g_ptr_array_new_with_free_func(arg_metadata_free);
}

static void
arguments_free(VALUE rb_arguments,
               GArray *in_args, GArray *out_args, GPtrArray *args_metadata)
{
    guint i;

    for (i = 0; i < args_metadata->len; i++) {
        RBGIArgMetadata *metadata;
        gint in_arg_index;

        metadata = g_ptr_array_index(args_metadata, i);
        if (metadata->direction == GI_DIRECTION_IN ||
            metadata->direction == GI_DIRECTION_INOUT) {
            in_arg_index = metadata->in_arg_index;
            if (in_arg_index != -1) {
                gint rb_arg_index;
                VALUE rb_argument = Qnil;
                GIArgument *argument;

                rb_arg_index = metadata->rb_arg_index;
                if (RARRAY_LEN(rb_arguments) > rb_arg_index) {
                    rb_argument = RARRAY_PTR(rb_arguments)[rb_arg_index];
                }
                argument = &(g_array_index(in_args, GIArgument, in_arg_index));
                rb_gi_in_argument_free(rb_argument,
                                       argument,
                                       &(metadata->arg_info));
            }
        } else {
            GIArgument *argument;
            argument = &(g_array_index(out_args, GIArgument,
                                       metadata->out_arg_index));
            rb_gi_out_argument_fin(argument, &(metadata->arg_info));
        }
    }

    g_array_unref(in_args);
    g_array_unref(out_args);
    g_ptr_array_unref(args_metadata);
}

typedef struct {
    GIFunctionInfo *info;
    GArray *in_args;
    GArray *out_args;
    GIArgument return_value;
    GError **error;
    gboolean succeeded;
} InvokeData;

static void
rb_gi_function_info_invoke_raw_call(InvokeData *data)
{
    data->succeeded =
        g_function_info_invoke(data->info,
                               (GIArgument *)((void *)(data->in_args->data)),
                               data->in_args->len,
                               (GIArgument *)((void *)(data->out_args->data)),
                               data->out_args->len,
                               &(data->return_value),
                               data->error);
}

static RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_TYPE
rb_gi_function_info_invoke_raw_call_without_gvl_body(void *user_data)
{
    InvokeData *data = (InvokeData *)user_data;

    rb_gi_function_info_invoke_raw_call(data);

    return RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_VALUE;
}

static gboolean
gobject_based_p(GIBaseInfo *info)
{
    GIBaseInfo *container_info;
    GIRegisteredTypeInfo *registered_type_info;

    container_info = g_base_info_get_container(info);
    if (g_base_info_get_type(container_info) != GI_INFO_TYPE_STRUCT) {
        return TRUE;
    }

    registered_type_info = (GIRegisteredTypeInfo *)container_info;
    if (g_registered_type_info_get_type_init(registered_type_info)) {
        return TRUE;
    }

    return FALSE;
}

VALUE
rb_gi_function_info_invoke_raw(GIFunctionInfo *info,
                               VALUE rb_info,
                               VALUE rb_receiver,
                               VALUE rb_arguments,
                               GIArgument *return_value,
                               VALUE *rb_return_value)
{
    GICallableInfo *callable_info;
    GIArgument receiver;
    GArray *in_args, *out_args;
    GPtrArray *args_metadata;
    VALUE rb_out_args = Qnil;
    gboolean succeeded;
    GError *error = NULL;
    gboolean unlock_gvl = FALSE;
    gboolean rb_receiver_is_class = FALSE;

    unlock_gvl = RVAL2CBOOL(rb_funcall(rb_info, rb_intern("unlock_gvl?"), 0));
    if (NIL_P(rb_receiver)) {
        receiver.v_pointer = NULL;
    } else {
        VALUE rb_receiver_class;
        rb_receiver_class = rb_class_of(rb_receiver);
        if (gobject_based_p((GIBaseInfo *)info) ||
            rb_respond_to(rb_receiver_class, rb_intern("gtype"))) {
            receiver.v_pointer = RVAL2GOBJ(rb_receiver);
        } else if (RVAL2CBOOL(rb_obj_is_kind_of(rb_receiver, rb_cClass)) &&
                   rb_respond_to(rb_receiver, rb_intern("gtype"))) {
            GObjectClass *object_class;
            rb_receiver_is_class = TRUE;
            object_class = g_type_class_ref(CLASS2GTYPE(rb_receiver));
            receiver.v_pointer = object_class;
        } else {
            receiver.v_pointer = rb_gi_struct_get_raw(rb_receiver, G_TYPE_NONE);
        }
    }
    rb_arguments = rbg_to_array(rb_arguments);

    callable_info = (GICallableInfo *)info;
    arguments_init(&in_args, &out_args, &args_metadata);
    if (receiver.v_pointer) {
        g_array_append_val(in_args, receiver);
    }
    arguments_from_ruby(callable_info, rb_receiver, rb_arguments,
                        in_args, out_args, args_metadata);
    {
        InvokeData data;
        data.info = info;
        data.in_args = in_args;
        data.out_args = out_args;
        data.error = &error;
        if (unlock_gvl) {
            rb_thread_call_without_gvl(
                rb_gi_function_info_invoke_raw_call_without_gvl_body, &data,
                NULL, NULL);
        } else {
            rb_gi_function_info_invoke_raw_call(&data);
        }
        succeeded = data.succeeded;

        if (rb_receiver_is_class) {
            g_type_class_unref(receiver.v_pointer);
        }

        if (return_value) {
            *return_value = data.return_value;
        }
        if (rb_return_value) {
            if (succeeded) {
                *rb_return_value = GI_RETURN_ARGUMENT2RVAL(callable_info,
                                                           &(data.return_value),
                                                           in_args,
                                                           out_args,
                                                           args_metadata);
            } else {
                *rb_return_value = Qnil;
            }
        }
    }

    if (succeeded) {
        rb_out_args = out_arguments_to_ruby(callable_info,
                                            in_args, out_args,
                                            args_metadata);
    }
    arguments_free(rb_arguments, in_args, out_args, args_metadata);
    if (!succeeded) {
        RG_RAISE_ERROR(error);
    }

    if (!NIL_P(rb_out_args) && RARRAY_LEN(rb_out_args) == 1) {
        VALUE rb_out_arg;
        rb_out_arg = RARRAY_PTR(rb_out_args)[0];
        if (rb_obj_is_kind_of(rb_out_arg, rb_eException)) {
            rb_exc_raise(rb_out_arg);
        }
    }

    return rb_out_args;
}

static VALUE
rg_invoke(VALUE self, VALUE rb_arguments)
{
    GIFunctionInfo *info;
    VALUE rb_out_args;
    VALUE rb_return_value;

    info = SELF(self);
    /* TODO: use rb_protect() */
    rb_out_args = rb_gi_function_info_invoke_raw(info,
                                                 self,
                                                 Qnil,
                                                 rb_arguments,
                                                 NULL,
                                                 &rb_return_value);

    if (NIL_P(rb_out_args)) {
        return rb_return_value;
    } else {
        GICallableInfo *callable_info = (GICallableInfo *)info;
        GITypeInfo return_value_info;
        g_callable_info_load_return_type(callable_info, &return_value_info);
        if (g_type_info_get_tag(&return_value_info) != GI_TYPE_TAG_VOID) {
            rb_ary_unshift(rb_out_args, rb_return_value);
        }
        if (RARRAY_LEN(rb_out_args) == 1) {
            return RARRAY_PTR(rb_out_args)[0];
        } else {
            return rb_out_args;
        }
    }
}

void
rb_gi_function_info_init(VALUE rb_mGI, VALUE rb_cGICallableInfo)
{
    rb_cGLibError = rb_const_get(mGLib, rb_intern("Error"));

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_FUNCTION_INFO, "FunctionInfo", rb_mGI,
				rb_cGICallableInfo);

    rb_iv_set(RG_TARGET_NAMESPACE, callbacks_key, rb_hash_new());

    callback_finders = g_ptr_array_new();
    rb_gi_callback_register_finder(source_func_callback_finder);

    RG_DEF_METHOD(set_unlock_gvl, 1);
    RG_DEF_METHOD_P(unlock_gvl, 0);

    RG_DEF_METHOD(symbol, 0);
    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(property, 0);
    RG_DEF_METHOD(vfunc, 0);
    RG_DEF_METHOD(invoke, 1);

    G_DEF_CLASS(G_TYPE_I_FUNCTION_INFO_FLAGS, "FunctionInfoFlags", rb_mGI);

    rb_gi_method_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_constructor_info_init(rb_mGI, RG_TARGET_NAMESPACE);
}
