/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2012  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project
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

#ifndef __RBGST_PRIVATE_H_
#define __RBGST_PRIVATE_H_

#include "rbgst.h"

char *_rbgst_inspect(VALUE object);
void _rbgst_define_class_if_need(VALUE klass, GType type, const gchar *prefix);
VALUE _rbgst_collect_elements(GstIterator *iter);

void _rbgst_mini_object_free(void *ptr);

/* implemented in rbgst-object.c */
VALUE rbgst_object_instance2robj(gpointer instance);


G_GNUC_INTERNAL void Init_gst_object(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_install_plugins_return(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_value(VALUE mGst);
G_GNUC_INTERNAL void Init_gst_element(VALUE mGst);

#endif /* __RBGST_PRIVATE_H_ */
