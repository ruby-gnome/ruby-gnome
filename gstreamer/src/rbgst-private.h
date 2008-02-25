/*
 *
 * Copyright (C) 2008 Ruby-GNOME2 Project
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#ifndef __RBGST_PRIVATE_H_
#define __RBGST_PRIVATE_H_

#include <ruby.h>

char *_rbgst_inspect(VALUE object);
void _rbgst_define_class_if_need(VALUE klass, GType type, const gchar *prefix);

void _rbgst_mini_object_free(void *ptr);

#endif /* __RBGST_PRIVATE_H_ */
