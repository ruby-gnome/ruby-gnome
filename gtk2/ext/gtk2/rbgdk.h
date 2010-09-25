/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkconversions.h -

  Copyright (C) 2008 Ruby-GNOME2 Project Team
************************************************/

#ifndef __RB_GDK_H__
#define __RB_GDK_H__

#include <rbgobject.h>
#include <rbgtkmacros.h>
#include <gdk/gdk.h>
#include <rbgdkconversions.h>

#ifdef GDK_WINDOWING_X11
#  include <gdk/gdkx.h>
#endif

#define GDK_BITMAP(b) ((GdkBitmap*)GDK_PIXMAP(b))

#define GEV2RVAL(ev) (make_gdkevent(ev))
#define RVAL2GEV(ev) (get_gdkevent(ev))
#define RVAL2ATOM(atom) (get_gdkatom(atom))

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

extern GType gdk_windowattr_get_type(void);
extern GType gdk_atom_get_type(void);
extern GType gdk_geometry_get_type(void);
extern GType gdk_region_get_type(void);
extern GType gdk_timecoord_get_type(void);

extern GdkAtom get_gdkatom(VALUE atom);
extern VALUE make_gdkevent(GdkEvent* event);
extern GdkEvent* get_gdkevent(VALUE event);

#endif /* __RB_GDK_H__ */
