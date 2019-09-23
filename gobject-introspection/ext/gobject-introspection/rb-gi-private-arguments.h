/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2019  Ruby-GNOME Project Team
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

typedef struct
{
    GICallableInfo *info;
    VALUE rb_receiver;
    gpointer receiver_type_class;
    VALUE rb_args;
    void **raw_args;
    GArray *in_args;
    GArray *out_args;
    GPtrArray *metadata;
} RBGIArguments;

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
rb_gi_arguments_fill_raw_results(RBGIArguments *args,
                                 VALUE rb_results,
                                 gpointer raw_return_value);

G_GNUC_INTERNAL VALUE
rb_gi_arguments_get_rb_in_args(RBGIArguments *args);
