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

#ifndef RB_GOBJECT_INTROSPECTION_H
#define RB_GOBJECT_INTROSPECTION_H

#include <ruby.h>
#include <rbgobject.h>

#include <girepository.h>

typedef gpointer (*RBGICallbackFinderFunc)(GIArgInfo *info);

typedef struct RBGICallbackData_ RBGICallbackData;
typedef struct RBGIArgMetadata_ RBGIArgMetadata;

void rb_gi_callback_data_free       (RBGICallbackData       *callback_data);
RBGIArgMetadata *
rb_gi_callback_data_get_metadata(RBGICallbackData *callback_data);
VALUE
rb_gi_callback_data_get_rb_callback(RBGICallbackData *callback_data);

void
rb_gi_callback_register_finder(RBGICallbackFinderFunc finder);

VALUE rb_gi_struct_new_raw(VALUE klass,
                           gpointer instance,
                           gboolean is_owned);
gpointer rb_gi_struct_get_raw(VALUE rb_struct,
                              GType gtype);

#endif
