/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2005 Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Geoff Youngs
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

#ifndef __RBGTKSOURCEVIEW3PRIVATE_H__
#define __RBGTKSOURCEVIEW3PRIVATE_H__

#include <gtksourceview/gtksource.h>
#include <gtksourceview/gtksourceview-typebuiltins.h>

#include <ruby.h>
#include <rb_cairo.h>
#include <rbgtk3.h>
#include "rbgtksourceview3conversions.h"
#include "rbgtksourceview3version.h"

extern void Init_gtksourceview3(void);

G_GNUC_INTERNAL void Init_gtksource_view (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_buffer (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_language (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_languagemanager (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_mark (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_printcompositor (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_style (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_stylescheme (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_styleschememanager (VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_gutter(VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_gutterrenderer(VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_gutterrendererpixbuf(VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_gutterrenderertext(VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_markattributes(VALUE mGtkSource);
G_GNUC_INTERNAL void Init_gtksource_undomanager(VALUE mGtkSource);

#endif /* __RBGTKSOURCEVIEW3PRIVATE_H__ */
