/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#ifndef __RBGDK3PRIVATE_H__
#define __RBGDK3PRIVATE_H__

#ifdef HAVE_RUBY_ST_H
#  include <ruby/st.h>
#else
#  include <st.h>
#endif

#include <rb_cairo.h>
#include <rbgobject.h>
#include "rbgdk3.h"

#ifndef HAVE_RB_ERRINFO
#  define rb_errinfo() (ruby_errinfo)
#endif

#ifndef G_VALUE_INIT
#  define G_VALUE_INIT { 0, { { 0 } } }
#endif

#if defined(HAVE_RB_CAIRO_H)
#  define CAIRO_AVAILABLE 1
#else
#  define CAIRO_AVAILABLE 0
#endif

extern ID id_call;

G_GNUC_INTERNAL void Init_gtk_gdk(void);
G_GNUC_INTERNAL void Init_gtk_gdk_atom(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_cairo(void);
G_GNUC_INTERNAL void Init_gtk_gdk_color(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_const(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_cursor(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_device(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_display(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_display_manager(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_dragcontext(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_event(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_geometry(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_keymap(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_keyval(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_pango(VALUE mGdk);
/* deprecated
G_GNUC_INTERNAL void Init_gtk_gdk_pangorenderer(VALUE mGdk);
*/
G_GNUC_INTERNAL void Init_gtk_gdk_pixbuf(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_property(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_rectangle(VALUE mGdk);
/* deprecated
G_GNUC_INTERNAL void Init_gtk_gdk_rgb(VALUE mGdk);
*/
G_GNUC_INTERNAL void Init_gdk_rgba(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_screen(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_selection(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_threads(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_timecoord(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_visual(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_window(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_windowattr(VALUE mGdk);
G_GNUC_INTERNAL void Init_gtk_gdk_x11(VALUE mGdk);
/* deprecated
G_GNUC_INTERNAL void Init_gtk_gdk_draw(VALUE mGdk);
*/

#endif /* __RBGDK3PRIVATE_H__ */
