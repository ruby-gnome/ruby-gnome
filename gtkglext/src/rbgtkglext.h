/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgtkglext.h,v 1.1 2003/08/17 10:45:46 isambart Exp $ */
/*
 * Copyright (C) 2003 Vincent Isambart <isambart@netcourrier.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _RBGTKGLEXT_H_included
#define _RBGTKGLEXT_H_included

#include "rbgtk.h"
#include <gdk/gdkgl.h>
#include <gtk/gtkgl.h>

void Init_gdk_gl_query(void);
void Init_gdk_gl_config(void);
void Init_gdk_gl_drawable(void);
void Init_gdk_gl_context(void);
void Init_gtk_gl_widget(void);
void Init_gdk_gl_pixmap(void);
void Init_gdk_gl_shapes(void);
void Init_gdk_gl_font(void);
void Init_gdk_gl_window(void);
void Init_gdk_gl_window(void);
void Init_gdk_gl_font(void);

extern VALUE mGdkGl;
extern VALUE mGtkGl;

#endif /* ! _RBGTKGLEXT_H_included */
