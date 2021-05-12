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

typedef struct RBGICallback_ {
    GIArgInfo *arg_info;
    GICallbackInfo *callback_info;
    gchar *method_name;
    ffi_cif cif;
    ffi_closure *closure;
} RBGICallback;

G_GNUC_INTERNAL void
rb_gi_callback_init(VALUE rb_mGI);

G_GNUC_INTERNAL gpointer
rb_gi_callback_find(GIArgInfo *info);

G_GNUC_INTERNAL RBGICallback *
rb_gi_callback_new(GICallbackInfo *callback_info,
                   const gchar *method_name);

G_GNUC_INTERNAL RBGICallbackData *
rb_gi_callback_data_new(RBGIArguments *args,
                        RBGICallback *callback,
                        RBGIArgMetadata *metadata);
