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
rb_gi_arguments_out_free_immediate_value(RBGIArguments *args,
                                         RBGIArgMetadata *metadata,
                                         gpointer user_data)
{
    xfree(metadata->out_arg->v_pointer);
}

static void
rb_gi_arguments_out_free_string(RBGIArguments *args,
                                RBGIArgMetadata *metadata,
                                gpointer user_data)
{
    gchar **target = metadata->out_arg->v_pointer;
    if (metadata->transfer != GI_TRANSFER_NOTHING) {
        g_free(*target);
    }
    xfree(target);
}

static void
rb_gi_arguments_out_free_array_c_uint8(RBGIArguments *args,
                                       RBGIArgMetadata *metadata,
                                       gpointer user_data)
{
    guint8 **target = metadata->out_arg->v_pointer;
    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        g_free(*target);
        break;
      default:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s free GIArgument(%s)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
    }
    xfree(target);
}

static void
rb_gi_arguments_out_free_array_c_string(RBGIArguments *args,
                                        RBGIArgMetadata *metadata,
                                        gpointer user_data)
{
    gchar ***target = metadata->out_arg->v_pointer;
    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        break;
      case GI_TRANSFER_CONTAINER:
        g_free(*target);
        break;
      case GI_TRANSFER_EVERYTHING:
        g_strfreev(*target);
        break;
      default:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s free GIArgument(%s)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
    }
    xfree(target);
}

static void
rb_gi_arguments_out_free_array_c_interface(RBGIArguments *args,
                                           RBGIArgMetadata *metadata,
                                           gpointer user_data)
{
    gpointer **target = metadata->out_arg->v_pointer;
    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        break;
      case GI_TRANSFER_CONTAINER:
        g_free(*target);
        break;
      case GI_TRANSFER_EVERYTHING:
      default:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s free GIArgument(%s/%s)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 rb_gi_array_type_to_string(metadata->array_type),
                 rb_gi_transfer_to_string(metadata->transfer));
    }
    xfree(target);
}

static void
rb_gi_arguments_out_free_array_array_interface_struct(RBGIArguments *args,
                                                      RBGIArgMetadata *metadata,
                                                      gpointer user_data)
{
    GArray *target = metadata->out_arg->v_pointer;
    GType gtype = metadata->element_type.interface_gtype;
    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        break;
      case GI_TRANSFER_CONTAINER:
        break;
      case GI_TRANSFER_EVERYTHING:
        if (gtype == G_TYPE_NONE) {
            /* If the target struct is raw (not GType-ed) struct, we
             * can't know how to free fields in the target struct. We
             * assume that the target struct doesn't allocate nothing
             * for its fields.
             *
             * e.g.: The attributes out argument in
             * vte_terminal_get_text_range():
             * https://developer.gnome.org/vte/unstable/VteTerminal.html#vte-terminal-get-text-range
             */
            break;
        }
      default:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s free GIArgument(%s/%s)[interface(%s)](%s)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 rb_gi_array_type_to_string(metadata->array_type),
                 g_info_type_to_string(metadata->element_type.interface_type),
                 g_type_name(metadata->element_type.interface_gtype),
                 rb_gi_transfer_to_string(metadata->transfer));
    }
    g_array_free(target, TRUE);
}

static void
rb_gi_arguments_out_free_interface_struct(RBGIArguments *args,
                                          RBGIArgMetadata *metadata,
                                          gpointer user_data)
{
    gpointer *target = metadata->out_arg->v_pointer;
    GType gtype = metadata->type.interface_gtype;
    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        if (gtype == G_TYPE_VALUE) {
            g_value_unset((GValue *)target);
            break;
        } else if (G_TYPE_IS_BOXED(gtype)) {
            if (*target) {
                g_boxed_free(gtype, *target);
            }
            break;
        }
      default:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s free GIArgument(%s)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
    }
    xfree(target);
}

static void
rb_gi_arguments_out_free_interface_object(RBGIArguments *args,
                                          RBGIArgMetadata *metadata,
                                          gpointer user_data)
{
    GObject **target = metadata->out_arg->v_pointer;
    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        break;
      case GI_TRANSFER_CONTAINER:
      case GI_TRANSFER_EVERYTHING:
        if (*target) {
            g_object_unref(*target);
        }
        break;
      default:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s free GIArgument(%s)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
    }
    xfree(target);
}

static void
rb_gi_arguments_out_free_list(RBGIArguments *args,
                              RBGIArgMetadata *metadata,
                              gpointer user_data)
{
    gpointer *target = metadata->out_arg->v_pointer;
    switch (metadata->transfer) {
      case GI_TRANSFER_NOTHING:
        break;
      case GI_TRANSFER_CONTAINER:
        if (metadata->type.tag == GI_TYPE_TAG_GLIST) {
            g_list_free(*((GList **)target));
        } else {
            g_slist_free(*((GSList **)target));
        }
        break;
      default:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s free GIArgument(%s)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
    }
    xfree(target);
}

static void
rb_gi_arguments_out_free_hash(RBGIArguments *args,
                              RBGIArgMetadata *metadata,
                              gpointer user_data)
{
    GHashTable *target = metadata->out_arg->v_pointer;
    if (metadata->transfer != GI_TRANSFER_NOTHING) {
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s free GIArgument(%s)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
    }
    xfree(target);
}

static void
rb_gi_arguments_out_free_error(RBGIArguments *args,
                               RBGIArgMetadata *metadata,
                               gpointer user_data)
{
    GError **target = metadata->out_arg->v_pointer;
    if (metadata->transfer != GI_TRANSFER_NOTHING) {
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s free GIArgument(%s)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->type.tag),
                 rb_gi_transfer_to_string(metadata->transfer));
    }
    xfree(target);
}

static void
rb_gi_arguments_out_init_arg_array_c(RBGIArguments *args,
                                     RBGIArgMetadata *metadata)
{
    GIArgument *argument = metadata->out_arg;
    switch (metadata->element_type.tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s allocates GIArgument(array)[c][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->element_type.tag));
        break;
      case GI_TYPE_TAG_UINT8:
        if (metadata->output_buffer_p) {
            if (!rb_type_p(metadata->rb_arg, RUBY_T_STRING)) {
                rb_raise(rb_eNotImpError,
                         "[%s::%s] %s allocates GIArgument(%s)[%s]: "
                         "output buffer isn't String: <%" PRIsVALUE ">",
                         args->name,
                         metadata->name,
                         rb_gi_direction_to_string(metadata->direction),
                         g_type_tag_to_string(metadata->type.tag),
                         rb_gi_transfer_to_string(metadata->transfer),
                         metadata->rb_arg);
            }
            RBGIArgMetadata *length_metadata = metadata->array_length_metadata;
            if (!length_metadata) {
                rb_raise(rb_eNotImpError,
                         "TODO: [%s::%s] %s allocates GIArgument(%s)[%s]: "
                         "output buffer without length",
                         args->name,
                         metadata->name,
                         rb_gi_direction_to_string(metadata->direction),
                         g_type_tag_to_string(metadata->type.tag),
                         rb_gi_transfer_to_string(metadata->transfer));
            }
            argument->v_pointer = RSTRING_PTR(metadata->rb_arg);
            length_metadata->out_arg->v_size = RSTRING_LEN(metadata->rb_arg);
        } else {
            argument->v_pointer = ALLOC(guint8 *);
            metadata->free_func = rb_gi_arguments_out_free_array_c_uint8;
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
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s allocates GIArgument(array)[c][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->element_type.tag));
        break;
      case GI_TYPE_TAG_UTF8:
        argument->v_pointer = ALLOC(gchar **);
        metadata->free_func = rb_gi_arguments_out_free_array_c_string;
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s allocates GIArgument(array)[c][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->element_type.tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        argument->v_pointer = ALLOC(gpointer *);
        metadata->free_func = rb_gi_arguments_out_free_array_c_interface;
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s allocates GIArgument(array)[c][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->element_type.tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_out_init_arg_array_array_interface(RBGIArguments *args,
                                                   RBGIArgMetadata *metadata)
{
    GIArgument *argument = metadata->out_arg;
    switch (metadata->element_type.interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s allocates "
                 "GIArgument(array)[array][interface(%s)](%s)",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->element_type.interface_type),
                 g_type_name(metadata->element_type.interface_gtype));
        break;
      case GI_INFO_TYPE_STRUCT:
        {
            gsize raw_struct_size;
            guint struct_size;

            raw_struct_size =
                g_struct_info_get_size(metadata->element_type.interface_info);
            struct_size = (guint)raw_struct_size;
            argument->v_pointer = g_array_new(metadata->zero_terminated_p,
                                              TRUE,
                                              struct_size);
            metadata->free_func =
                rb_gi_arguments_out_free_array_array_interface_struct;
        }
        break;
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
                 "TODO: [%s] %s allocates "
                 "GIArgument(array)[array][interface(%s)](%s)",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->element_type.interface_type),
                 g_type_name(metadata->element_type.interface_gtype));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_out_init_arg_array_array(RBGIArguments *args,
                                         RBGIArgMetadata *metadata)
{
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
                 "TODO: [%s] %s allocates GIArgument(array)[array][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->element_type.tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_out_init_arg_array_array_interface(args, metadata);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s allocates GIArgument(array)[array][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->element_type.tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_out_init_arg_array(RBGIArguments *args,
                                   RBGIArgMetadata *metadata)
{
    switch (metadata->array_type) {
      case GI_ARRAY_TYPE_C:
        rb_gi_arguments_out_init_arg_array_c(args, metadata);
        break;
      case GI_ARRAY_TYPE_ARRAY:
        rb_gi_arguments_out_init_arg_array_array(args, metadata);
        break;
      case GI_ARRAY_TYPE_PTR_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s allocates GIArgument(array)[ptr-array][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->element_type.tag));
        break;
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: [%s] %s allocates GIArgument(array)[byte-array][%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_type_tag_to_string(metadata->element_type.tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_out_init_arg_interface(RBGIArguments *args,
                                       RBGIArgMetadata *metadata)
{
    GIArgument *argument = metadata->out_arg;
    switch (metadata->type.interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(interface)[%s] for output",
                 g_info_type_to_string(metadata->type.interface_type));
        break;
      case GI_INFO_TYPE_STRUCT:
        /* Should we care gtype? */
        if (metadata->type.pointer_p) {
            gpointer *struct_location = RB_ALLOC(gpointer);
            *struct_location = NULL;
            argument->v_pointer = struct_location;
        } else {
            gsize struct_size =
                g_struct_info_get_size(metadata->type.interface_info);
            argument->v_pointer = xmalloc(struct_size);
            memset(argument->v_pointer, 0, struct_size);
        }
        metadata->free_func = rb_gi_arguments_out_free_interface_struct;
        break;
      case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(interface)[%s] for output",
                 g_info_type_to_string(metadata->type.interface_type));
        break;
      case GI_INFO_TYPE_ENUM:
        {
            gint *pointer = ALLOC(gint);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_INFO_TYPE_FLAGS:
        {
            guint *pointer = ALLOC(guint);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        {
            gpointer *pointer = ALLOC(gpointer);
            *pointer = NULL;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_interface_object;
        }
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
                 "TODO: [%s] %s allocates GIArgument(interface)[%s]",
                 metadata->name,
                 rb_gi_direction_to_string(metadata->direction),
                 g_info_type_to_string(metadata->type.interface_type));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_out_init_arg(RBGIArguments *args,
                             RBGIArgMetadata *metadata)
{
    GIArgument *argument = metadata->out_arg;

    if (metadata->array_length_p &&
        metadata->array_metadata &&
        metadata->array_metadata->output_buffer_p) {
        return;
    }

    memset(argument, 0, sizeof(GIArgument));
    switch (metadata->type.tag) {
      case GI_TYPE_TAG_VOID:
        if (metadata->type.pointer_p) {
            gpointer *pointer = ALLOC(gpointer);
            *pointer = NULL;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_BOOLEAN:
        {
            gboolean *pointer = ALLOC(gboolean);
            *pointer = FALSE;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_INT8:
        {
            gint8 *pointer = ALLOC(gint8);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_UINT8:
        {
            guint8 *pointer = ALLOC(guint8);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_INT16:
        {
            gint16 *pointer = ALLOC(gint16);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_UINT16:
        {
            guint16 *pointer = ALLOC(guint16);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_INT32:
        {
            gint32 *pointer = ALLOC(gint32);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_UINT32:
        {
            guint32 *pointer = ALLOC(guint32);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_INT64:
        {
            gint64 *pointer = ALLOC(gint64);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_UINT64:
        {
            guint64 *pointer = ALLOC(guint64);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_FLOAT:
        {
            gfloat *pointer = ALLOC(gfloat);
            *pointer = 0.0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_DOUBLE:
        {
            gdouble *pointer = ALLOC(gdouble);
            *pointer = 0.0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_GTYPE:
        {
            GType *pointer = ALLOC(GType);
            *pointer = G_TYPE_INVALID;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        {
            gchar **pointer = ALLOC(gchar *);
            *pointer = NULL;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_string;
        }
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_gi_arguments_out_init_arg_array(args, metadata);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_out_init_arg_interface(args, metadata);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
        {
            gpointer *pointer = ALLOC(gpointer);
            *pointer = NULL;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_list;
        }
        break;
      case GI_TYPE_TAG_GHASH:
        {
            gpointer *pointer = ALLOC(gpointer);
            *pointer = NULL;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_hash;
        }
        break;
      case GI_TYPE_TAG_ERROR:
        {
            GError **pointer = ALLOC(GError *);
            *pointer = NULL;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_error;
        }
        break;
      case GI_TYPE_TAG_UNICHAR:
        {
            gunichar *pointer = ALLOC(gunichar);
            *pointer = 0;
            argument->v_pointer = pointer;
            metadata->free_func = rb_gi_arguments_out_free_immediate_value;
        }
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

void
rb_gi_arguments_out_init(RBGIArguments *args)
{
    guint i;

    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;

        metadata = g_ptr_array_index(args->metadata, i);
        if (metadata->direction != GI_DIRECTION_OUT) {
            continue;
        }

        if (args->rb_mode_p) {
            rb_gi_arguments_out_init_arg(args, metadata);
        } else {
            GIArgument *argument = &(g_array_index(args->out_args,
                                                   GIArgument,
                                                   metadata->out_arg_index));
            argument->v_pointer = *((gpointer *)(args->raw_args[i]));
        }
    }
}

void
rb_gi_arguments_out_clear(RBGIArguments *args)
{
    guint i;

    for (i = 0; i < args->metadata->len; i++) {
        RBGIArgMetadata *metadata;

        metadata = g_ptr_array_index(args->metadata, i);
        if (metadata->direction != GI_DIRECTION_OUT) {
            continue;
        }

        if (metadata->free_func) {
            metadata->free_func(args, metadata, metadata->free_func_data);
        }
    }
}

static VALUE
rb_gi_arguments_out_to_ruby_arg(RBGIArguments *args,
                                RBGIArgMetadata *metadata,
                                GIArgument *argument)
{
    /* TODO */
    if (metadata->output_buffer_p) {
        return metadata->rb_arg;
    }

    GIArgument normalized_argument;
    gboolean duplicate = FALSE;

    memset(&normalized_argument, 0, sizeof(GIArgument));
    switch (metadata->type.tag) {
      case GI_TYPE_TAG_VOID:
        if (metadata->type.pointer_p) {
            normalized_argument.v_pointer = *((gpointer *)(argument->v_pointer));
        }
        break;
      case GI_TYPE_TAG_BOOLEAN:
        normalized_argument.v_boolean = *((gboolean *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_INT8:
        normalized_argument.v_int8 = *((gint8 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UINT8:
        normalized_argument.v_uint8 = *((guint8 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_INT16:
        normalized_argument.v_int16 = *((gint16 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UINT16:
        normalized_argument.v_uint16 = *((guint16 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_INT32:
        normalized_argument.v_int32 = *((gint32 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UINT32:
        normalized_argument.v_uint32 = *((guint32 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_INT64:
        normalized_argument.v_int64 = *((gint64 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UINT64:
        normalized_argument.v_uint64 = *((guint64 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_FLOAT:
        normalized_argument.v_float = *((gfloat *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_DOUBLE:
        normalized_argument.v_double = *((gdouble *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_GTYPE:
        normalized_argument.v_size = *((GType *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        normalized_argument.v_string = *((gchar **)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_ARRAY:
      case GI_TYPE_TAG_INTERFACE:
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        if (metadata->caller_allocates_p) {
            duplicate = TRUE;
            normalized_argument.v_pointer = argument->v_pointer;
        } else {
            normalized_argument.v_pointer = *((gpointer *)(argument->v_pointer));
        }
        break;
      case GI_TYPE_TAG_ERROR:
        normalized_argument.v_pointer = *((GError **)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UNICHAR:
        normalized_argument.v_uint32 = *((guint32 *)(argument->v_pointer));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    return rb_gi_argument_to_ruby(&normalized_argument,
                                  duplicate,
                                  metadata->type.info,
                                  args->in_args,
                                  args->out_args,
                                  args->metadata);
}

VALUE
rb_gi_arguments_out_to_ruby(RBGIArguments *args)
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

        rb_argument = rb_gi_arguments_out_to_ruby_arg(args,
                                                      metadata,
                                                      argument);
        rb_ary_push(rb_out_args, rb_argument);
    }

    if (RARRAY_LEN(rb_out_args) == 0) {
        return Qnil;
    } else {
        return rb_out_args;
    }
}
