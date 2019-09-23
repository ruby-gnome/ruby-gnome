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
rb_gi_arguments_out_init_arg_array_c(RBGIArguments *args,
                                     RBGIArgMetadata *metadata)
{
    GIArgument *argument = &(g_array_index(args->out_args,
                                           GIArgument,
                                           metadata->out_arg_index));
    switch (metadata->element_type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(array)[c][%s] for output",
                 g_type_tag_to_string(metadata->element_type_tag));
        break;
      case GI_TYPE_TAG_UINT8:
        argument->v_pointer = xmalloc(sizeof(guint8 *));
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
                 "TODO: allocates GIArgument(array)[c][%s] for output",
                 g_type_tag_to_string(metadata->element_type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
        argument->v_pointer = xmalloc(sizeof(gchar **));
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(array)[c][%s] for output",
                 g_type_tag_to_string(metadata->element_type_tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        argument->v_pointer = xmalloc(sizeof(gpointer *));
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(array)[c][%s] for output",
                 g_type_tag_to_string(metadata->element_type_tag));
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
    GIArgument *argument = &(g_array_index(args->out_args,
                                           GIArgument,
                                           metadata->out_arg_index));
    switch (metadata->element_interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[array][interface(%s)](%s) -> Ruby",
                 g_info_type_to_string(metadata->element_interface_type),
                 g_type_name(metadata->element_interface_gtype));
        break;
      case GI_INFO_TYPE_STRUCT:
        {
            gsize raw_struct_size;
            guint struct_size;

            raw_struct_size =
                g_struct_info_get_size(metadata->element_interface_info);
            struct_size = (guint)raw_struct_size;
            argument->v_pointer = g_array_new(metadata->zero_terminated_p,
                                              TRUE,
                                              struct_size);
            break;
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
                 g_info_type_to_string(metadata->element_interface_type),
                 g_type_name(metadata->element_interface_gtype));
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
    switch (metadata->element_type_tag) {
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
                 "TODO: allocates GIArgument(array)[array][%s] for output",
                 g_type_tag_to_string(metadata->element_type_tag));
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
                 "TODO: allocates GIArgument(array)[array][%s] for output",
                 g_type_tag_to_string(metadata->element_type_tag));
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
                 "TODO: allocates GIArgument(array)[ptr-array][%s] for output",
                 g_type_tag_to_string(metadata->element_type_tag));
        break;
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(array)[byte-array][%s] for output",
                 g_type_tag_to_string(metadata->element_type_tag));
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
    GIArgument *argument = &(g_array_index(args->out_args,
                                           GIArgument,
                                           metadata->out_arg_index));
    /* TODO: Can we use the following code? */
    /*
    if (!metadata->caller_allocates_p) {
        argument->v_pointer = ALLOC(gpointer);
        return;
    }
    */

    switch (metadata->interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(interface)[%s] for output",
                 g_info_type_to_string(metadata->interface_type));
        break;
      case GI_INFO_TYPE_STRUCT:
        {
            gsize struct_size;

            /* Should we care gtype?
               Related: rb_gi_arguments_out_arg_clear_interface() */
            struct_size = g_struct_info_get_size(metadata->interface_info);
            argument->v_pointer = xmalloc(struct_size);
            memset(argument->v_pointer, 0, struct_size);
        }
        break;
      case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(interface)[%s] for output",
                 g_info_type_to_string(metadata->interface_type));
        break;
      case GI_INFO_TYPE_ENUM:
        {
            gint *pointer = ALLOC(gint);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_INFO_TYPE_FLAGS:
        {
            guint *pointer = ALLOC(guint);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        {
            gpointer *pointer = ALLOC(gpointer);
            *pointer = NULL;
            argument->v_pointer = pointer;
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
                 "TODO: allocates GIArgument(interface)[%s] for output",
                 g_info_type_to_string(metadata->interface_type));
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
    GIArgument *argument = &(g_array_index(args->out_args,
                                           GIArgument,
                                           metadata->out_arg_index));
    memset(argument, 0, sizeof(GIArgument));
    switch (metadata->type_tag) {
      case GI_TYPE_TAG_VOID:
        if (metadata->pointer_p) {
            gpointer *pointer = ALLOC(gpointer);
            *pointer = NULL;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_BOOLEAN:
        {
            gboolean *pointer = ALLOC(gboolean);
            *pointer = FALSE;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_INT8:
        {
            gint8 *pointer = ALLOC(gint8);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UINT8:
        {
            guint8 *pointer = ALLOC(guint8);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_INT16:
        {
            gint16 *pointer = ALLOC(gint16);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UINT16:
        {
            guint16 *pointer = ALLOC(guint16);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_INT32:
        {
            gint32 *pointer = ALLOC(gint32);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UINT32:
        {
            guint32 *pointer = ALLOC(guint32);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_INT64:
        {
            gint64 *pointer = ALLOC(gint64);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UINT64:
        {
            guint64 *pointer = ALLOC(guint64);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_FLOAT:
        {
            gfloat *pointer = ALLOC(gfloat);
            *pointer = 0.0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_DOUBLE:
        {
            gdouble *pointer = ALLOC(gdouble);
            *pointer = 0.0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_GTYPE:
        {
            GType *pointer = ALLOC(GType);
            *pointer = G_TYPE_INVALID;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        {
            gchar **pointer = ALLOC(gchar *);
            *pointer = NULL;
            argument->v_pointer = pointer;
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
      case GI_TYPE_TAG_GHASH:
        {
            gpointer *pointer = ALLOC(gpointer);
            *pointer = NULL;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_ERROR:
        {
            GError **pointer = ALLOC(GError *);
            *pointer = NULL;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UNICHAR:
        {
            gunichar *pointer = ALLOC(gunichar);
            *pointer = 0;
            argument->v_pointer = pointer;
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

static void
rb_gi_arguments_out_clear_arg_array_c(RBGIArguments *args,
                                      RBGIArgMetadata *metadata)
{
    GIArgument *argument = &(g_array_index(args->out_args,
                                           GIArgument,
                                           metadata->out_arg_index));
    switch (metadata->element_type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(array)[c][%s][%s]",
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_TYPE_TAG_UINT8:
        if (metadata->transfer == GI_TRANSFER_EVERYTHING) {
            g_free(*((guint8 **)(argument->v_pointer)));
        }
        xfree(argument->v_pointer);
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
                 "TODO: free out GIArgument(array)[c][%s][%s]",
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_TYPE_TAG_UTF8:
        if (metadata->transfer != GI_TRANSFER_NOTHING) {
            g_free(*((gchar **)argument->v_pointer));
        }
        xfree(argument->v_pointer);
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(array)[c][%s][%s]",
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_TYPE_TAG_INTERFACE:
        if (metadata->transfer == GI_TRANSFER_EVERYTHING) {
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(array)[c][%s][%s]",
                     g_type_tag_to_string(metadata->element_type_tag),
                     rb_gi_transfer_to_string(metadata->transfer));
        }
        if (metadata->transfer != GI_TRANSFER_NOTHING) {
            g_free(*((gpointer *)argument->v_pointer));
        }
        xfree(argument->v_pointer);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(array)[c][%s][%s]",
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_out_clear_arg_array(RBGIArguments *args,
                                    RBGIArgMetadata *metadata)
{
    GIArgument *argument = &(g_array_index(args->out_args,
                                           GIArgument,
                                           metadata->out_arg_index));
    switch (metadata->array_type) {
      case GI_ARRAY_TYPE_C:
        rb_gi_arguments_out_clear_arg_array_c(args, metadata);
        break;
      case GI_ARRAY_TYPE_ARRAY:
        if (metadata->transfer != GI_TRANSFER_NOTHING) {
            rb_raise(rb_eNotImpError,
                     "TODO: "
                     "free out transfer GIArgument(array)[ptr-array][%s][%s]",
                     g_type_tag_to_string(metadata->element_type_tag),
                     rb_gi_transfer_to_string(metadata->transfer));
        }
        g_array_free(argument->v_pointer, TRUE);
        break;
      case GI_ARRAY_TYPE_PTR_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(array)[ptr-array][%s][%s]",
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(array)[byte-array][%s][%s]",
                 g_type_tag_to_string(metadata->element_type_tag),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_out_clear_arg_interface(RBGIArguments *args,
                                        RBGIArgMetadata *metadata)
{
    GIArgument *argument = &(g_array_index(args->out_args,
                                           GIArgument,
                                           metadata->out_arg_index));
    switch (metadata->interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: free out transfer GIArgument(interface)[%s][%s]",
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_INFO_TYPE_STRUCT:
        /* Should we care gtype?
           Related: rb_gi_arguments_out_init_arg_interface() */
        xfree(argument->v_pointer);
        break;
      case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: free out transfer GIArgument(interface)[%s][%s]",
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
        break;
      case GI_INFO_TYPE_OBJECT:
        switch (metadata->transfer) {
          case GI_TRANSFER_NOTHING:
            break;
          case GI_TRANSFER_CONTAINER:
          case GI_TRANSFER_EVERYTHING:
            {
                GObject *object = *((GObject **)argument->v_pointer);
                if (object) {
                    g_object_unref(object);
                }
            }
            break;
          default:
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(interface)[%s][%s]",
                     g_info_type_to_string(metadata->interface_type),
                     rb_gi_transfer_to_string(metadata->transfer));
            break;
        }
        xfree(argument->v_pointer);
        break;
      case GI_INFO_TYPE_INTERFACE:
        if (metadata->transfer != GI_TRANSFER_NOTHING) {
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(interface)[%s][%s]",
                     g_info_type_to_string(metadata->interface_type),
                     rb_gi_transfer_to_string(metadata->transfer));
        }
        xfree(argument->v_pointer);
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
                 "TODO: free out transfer GIArgument(interface)[%s][%s]",
                 g_info_type_to_string(metadata->interface_type),
                 rb_gi_transfer_to_string(metadata->transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_arguments_out_clear_arg(RBGIArguments *args,
                              RBGIArgMetadata *metadata)
{
    GIArgument *argument = &(g_array_index(args->out_args,
                                           GIArgument,
                                           metadata->out_arg_index));
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
        xfree(argument->v_pointer);
        break;
      case GI_TYPE_TAG_UTF8:
        if (metadata->transfer != GI_TRANSFER_NOTHING) {
            g_free(*((gchar **)argument->v_pointer));
        }
        xfree(argument->v_pointer);
        break;
      case GI_TYPE_TAG_FILENAME:
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(%s)",
                 g_type_tag_to_string(metadata->type_tag));
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_gi_arguments_out_clear_arg_array(args, metadata);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_arguments_out_clear_arg_interface(args, metadata);
        break;
      case GI_TYPE_TAG_GLIST:
        if (metadata->transfer == GI_TRANSFER_EVERYTHING) {
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(%s)[%s]",
                     g_type_tag_to_string(metadata->type_tag),
                     rb_gi_transfer_to_string(metadata->transfer));
        }
        if (metadata->transfer != GI_TRANSFER_NOTHING) {
            g_list_free(*((GList **)argument->v_pointer));
        }
        xfree(argument->v_pointer);
        break;
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
        if (metadata->transfer != GI_TRANSFER_NOTHING) {
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(%s)[%s]",
                     g_type_tag_to_string(metadata->type_tag),
                     rb_gi_transfer_to_string(metadata->transfer));
        }
        xfree(argument->v_pointer);
        break;
      case GI_TYPE_TAG_UNICHAR:
        xfree(argument->v_pointer);
        break;
      default:
        g_assert_not_reached();
        break;
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

        if (!args->rb_mode_p) {
            continue;
        }

        rb_gi_arguments_out_clear_arg(args, metadata);
    }
}

static VALUE
rb_gi_arguments_out_to_ruby_arg(RBGIArguments *args,
                                RBGIArgMetadata *metadata,
                                GIArgument *argument)
{
    GIArgument normalized_argument;
    gboolean duplicate = FALSE;

    memset(&normalized_argument, 0, sizeof(GIArgument));
    switch (metadata->type_tag) {
      case GI_TYPE_TAG_VOID:
        if (metadata->pointer_p) {
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
                                  &(metadata->type_info),
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
