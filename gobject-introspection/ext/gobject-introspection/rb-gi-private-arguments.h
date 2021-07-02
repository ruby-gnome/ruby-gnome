/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2019-2021  Ruby-GNOME Project Team
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

#pragma once

typedef struct {
    GICallableInfo *info;
    const gchar *namespace;
    const gchar *name;
    VALUE rb_receiver;
    gpointer receiver_type_class;
    VALUE rb_args;
    void **raw_args;
    gboolean rb_mode_p;
    GArray *in_args;
    GArray *out_args;
    GPtrArray *metadata;
} RBGIArguments;

typedef void (*RBGIArgFreeFunc)(RBGIArguments *args,
                                RBGIArgMetadata *metadata,
                                gpointer user_data);

typedef struct {
    GITypeInfo *info;
    gboolean pointer_p;
    GITypeTag tag;
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GType interface_gtype;
} RBGIArgMetadataType;

struct RBGIArgMetadata_ {
    GICallableInfo *callable_info;
    GIArgInfo arg_info;
    const gchar *name;
    RBGIArgMetadataType type;
    RBGIArgMetadataType element_type;
    RBGIArgMetadataType key_type;
    RBGIArgMetadataType value_type;
    GIScopeType scope_type;
    GIDirection direction;
    GITransfer transfer;
    gboolean callback_p;
    gboolean closure_p;
    gboolean destroy_p;
    gboolean array_p;
    gboolean array_length_p;
    gboolean interface_p;
    gboolean may_be_null_p;
    gboolean caller_allocates_p;
    gboolean zero_terminated_p;
    gboolean output_buffer_p;
    GIArrayType array_type;
    gint index;
    gint in_arg_index;
    gint closure_in_arg_index;
    gint destroy_in_arg_index;
    gint rb_arg_index;
    gint out_arg_index;
    GIArgument *in_arg;
    GIArgument *out_arg;
    VALUE rb_arg;
    RBGIArgMetadata *array_metadata;
    RBGIArgMetadata *array_length_metadata;
    GIArgument *array_length_arg;
    RBGIArgFreeFunc free_func;
    gpointer free_func_data;
};

G_GNUC_INTERNAL void
rb_gi_arguments_init(RBGIArguments *args,
                     GICallableInfo *info,
                     VALUE rb_receiver,
                     VALUE rb_args,
                     void **raw_args);
G_GNUC_INTERNAL void
rb_gi_arguments_clear(RBGIArguments *args);

G_GNUC_INTERNAL VALUE
rb_gi_arguments_get_rb_out_args(RBGIArguments *args);

G_GNUC_INTERNAL VALUE
rb_gi_arguments_get_rb_return_value(RBGIArguments *args,
                                    GIArgument *return_value);

G_GNUC_INTERNAL void
rb_gi_arguments_fill_raw_out_gerror(RBGIArguments *args,
                                    VALUE rb_error);

G_GNUC_INTERNAL void
rb_gi_arguments_fill_raw_results(RBGIArguments *args,
                                 VALUE rb_results,
                                 gpointer raw_return_value);
