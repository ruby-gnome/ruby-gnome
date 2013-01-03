/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2013  Ruby-GNOME2 Project Team
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

#ifndef RB_CAIRO_GOBJECT_H
#define RB_CAIRO_GOBJECT_H

#include <ruby.h>
#include <rbgobject.h>
#include <glib-enum-types.h>

#include <cairo-gobject.h>

#ifndef CAIRO_GOBJECT_TYPE_HINT_METRICS
#  define CAIRO_GOBJECT_TYPE_HINT_METRICS CAIRO_GOBJECT_TYPE_HNT_METRICS
#endif

#ifndef CAIRO_GOBJECT_TYPE_TEXT_CLUSTER_FLAGS
#  define CAIRO_GOBJECT_TYPE_TEXT_CLUSTER_FLAGS cairo_gobject_text_cluster_flags_get_type ()
#endif

#include <rb_cairo.h>

extern void Init_cairo_gobject(void);

#endif
