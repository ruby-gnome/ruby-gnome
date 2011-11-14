/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
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

#ifndef __RB_GDK_H__
#define __RB_GDK_H__

#include <rbgobject.h>
#include <rbgtkmacros.h>
#include <gdk/gdk.h>
#include <rbgdkconversions.h>

#ifdef GDK_WINDOWING_X11
#  include <gdk/gdkx.h>
#endif
#ifdef GDK_WINDOWING_WIN32
#  include <gdk/gdkwin32.h>
#endif

#define RVAL2GDKPOINTS(value, n) rbgdk_rval2gdkpoints(value, n)
#define RVAL2GDKATOMS(value, n) rbgdk_rval2gdkatoms(value, n)

#define RVAL2GDKPIXBUFGLIST(value) rbgdk_rval2gdkpixbufglist(value)

#define GDK_BITMAP(b) ((GdkBitmap*)GDK_PIXMAP(b))

#define GEV2RVAL(ev) (make_gdkevent(ev))
#define RVAL2GEV(ev) (get_gdkevent(ev))
#define RVAL2ATOM(atom) (get_gdkatom(atom))
#define RVAL2GDKNATIVEWINDOW(value) ((GdkNativeWindow)NUM2UINT(value))
/* TODO: Will UINT2NUM work for pointers on 64-bit Windows? */
#define GDKNATIVEWINDOW2RVAL(value) UINT2NUM((guint32)value)

#define GDK_TYPE_GEOMETRY (gdk_geometry_get_type())
#define GDK_TYPE_REGION (gdk_region_get_type())
#define GDK_TYPE_ATOM (gdk_atom_get_type())
#define GDK_TYPE_WINDOW_ATTR (gdk_windowattr_get_type())
#define GDK_TYPE_TIME_COORD (gdk_timecoord_get_type())

RUBY_GTK2_VAR VALUE mGdk;
RUBY_GTK2_VAR VALUE rb_cGdkColor;

typedef struct {
    GdkAtom atom;
} GdkAtomData;

GdkPoint *rbgdk_rval2gdkpoints(VALUE value, long *n);
GdkAtom *rbgdk_rval2gdkatoms(VALUE value, long *n);

GList *rbgdk_rval2gdkpixbufglist(VALUE value);

extern GType gdk_windowattr_get_type(void);
extern GType gdk_atom_get_type(void);
extern GType gdk_geometry_get_type(void);
extern GType gdk_region_get_type(void);
extern GType gdk_timecoord_get_type(void);

extern GdkAtom get_gdkatom(VALUE atom);
extern VALUE make_gdkevent(GdkEvent* event);
extern GdkEvent* get_gdkevent(VALUE event);

#endif /* __RB_GDK_H__ */
