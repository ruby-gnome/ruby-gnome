/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtk.h -

  $Author: mutoh $
  $Date: 2002/09/14 15:43:40 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef _RBGTK_H
#define _RBGTK_H

#include "ruby.h"
#include "rubysig.h"
#include "rbglib.h"
#include "rbgobject.h"
#include <gtk/gtk.h>

/* for GDK_ROOT_WINDOW() */
#if defined GDK_WINDOWING_X11
# include <gdk/gdkx.h>
#elif defined GDK_WINDOWING_FB
# include <gdk/linux-fb/gdkfb.h>
#elif defined GDK_WINDOWING_DIRECTFB
# include <gdk/directfb/gdkdirectfb.h>
/*#elif defined(GDK_WINDOWING_BEOS)*/
#elif defined GDK_WINDOWING_WIN32
# if !defined HWND_DESKTOP
#  define HWND_DESKTOP 0
# endif
# define GDK_ROOT_WINDOW() ((guint32) HWND_DESKTOP)
#endif

#include <signal.h>

#define GDK_BITMAP(b) ((GdkBitmap*)GDK_PIXMAP(b))

#define GEV2RVAL(ev) (make_gdkevent(ev))
#define RVAL2GEV(ev) (get_gdkevent(ev))

#define RBGTK_INITIALIZE(obj,gtkobj)\
 (rbgtk_initialize_gtkobject(obj, GTK_OBJECT(gtkobj)))

#define GDK_TYPE_GEOMETRY (gdk_geometry_get_type())
#define GDK_TYPE_REGION (gdk_region_get_type())
#define GDK_TYPE_ATOM (gdk_atom_get_type())


extern VALUE glist2ary(GList* list);
extern GList* ary2glist(VALUE ary);
extern GSList* ary2gslist(VALUE ary);
extern VALUE gslist2ary(GSList *list);

extern ID id_relative_callbacks;
extern ID id_call;

extern VALUE mGtk;
extern VALUE mGdk;
extern VALUE mEditable;

/*
 * for gtk
 */
extern void rbgtk_initialize_gtkobject(VALUE obj, GtkObject *gtkobj);
extern void exec_callback(GtkWidget *widget, gpointer proc);

/*
 * for gdk
 */
typedef struct {
  GdkAtom atom;
} GdkAtomData;

extern GType gdk_atom_get_type(void);
extern GType gdk_geometry_get_type(void);
extern GType gdk_region_get_type(void);

extern VALUE make_gdkevent(GdkEvent* event);
extern GdkEvent* get_gdkevent(VALUE event);

#endif /* _RBGTK_H */
