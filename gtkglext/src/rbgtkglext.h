/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgtkglext.h,v 1.2 2003/08/20 22:36:02 isambart Exp $ */
/* GtkGLExt main header
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

void Init_gtkglext_gdk_gl_query(void);   /* Gdk::GL - query  */
void Init_gtkglext_gdk_gl_font(void);    /* Gdk::GL - fonts  */
void Init_gtkglext_gdk_gl_shapes(void);  /* Gdk::GL - shapes */

void Init_gtkglext_gdk_glconfig(void);   /* Gdk::GLConfig   */
void Init_gtkglext_gdk_gldrawable(void); /* Gdk::GLDrawable */
void Init_gtkglext_gdk_glcontext(void);  /* Gdk::GLContext  */
void Init_gtkglext_gdk_glpixmap(void);   /* Gdk::GLPixmap   */
void Init_gtkglext_gdk_glwindow(void);   /* Gdk::GLWindow   */
void Init_gtkglext_gtk_widget(void);     /* Gtk::Widget     */
void Init_gtkglext_gdk_pixmap(void);     /* Gdk::Pixmap     */
void Init_gtkglext_gdk_window(void);     /* Gdk::Window     */

extern VALUE mGdk, mGdkGL;
extern VALUE mGtk, mGtkGL;

#endif /* ! _RBGTKGLEXT_H_included */
