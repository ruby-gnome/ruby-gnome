/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtk.h -

  $Author: sakai $
  $Date: 2002/08/30 18:24:47 $

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

#define CSTR2OBJ(s) (s ? rb_str_new2(s) : Qnil)
#define RBGTK_INITIALIZE(obj,gtkobj)\
 (rbgtk_initialize_gtkobject(obj, GTK_OBJECT(gtkobj)))

extern VALUE glist2ary(GList* list);
extern GList* ary2glist(VALUE ary);
extern GSList* ary2gslist(VALUE ary);
extern VALUE gslist2ary(GSList *list);
extern VALUE arg_to_value(GtkArg* arg);

extern ID id_relative_callbacks;
extern ID id_call;

extern VALUE mRC;
extern VALUE mGtk;
extern VALUE mGtkDrag;
extern VALUE mEditable;

extern VALUE gRequisition;

extern VALUE mGdk;
extern VALUE mGdkKeyval;
extern VALUE mGdkSelection;
extern VALUE mGdkRgb;

extern VALUE gdkRegion;
extern VALUE gdkAtom;
extern VALUE gdkEvent;

extern VALUE gdkEventType;
extern VALUE gdkEventAny;
extern VALUE gdkEventExpose;
extern VALUE gdkEventNoExpose;
extern VALUE gdkEventVisibility;
extern VALUE gdkEventMotion;
extern VALUE gdkEventButton;
extern VALUE gdkEventKey;
extern VALUE gdkEventCrossing;
extern VALUE gdkEventFocus;
extern VALUE gdkEventConfigure;
extern VALUE gdkEventProperty;
extern VALUE gdkEventSelection;
extern VALUE gdkEventDND;
extern VALUE gdkEventProximity;
extern VALUE gdkEventClient;
extern VALUE gdkEventOther;

/*
 * for gtk
 */
extern void rbgtk_initialize_gtkobject(VALUE obj, GtkObject *gtkobj);

extern void exec_callback(GtkWidget *widget, gpointer proc);

extern VALUE make_gtkprevinfo(GtkPreviewInfo* info);
extern GtkPreviewInfo* get_gtkprevinfo(VALUE value);

#define add_relative rbgobj_add_relative
#define add_relative_removable rbgobj_add_relative_removable
#define remove_relative rbgobj_remove_relative

/*
 * for gdk
 */
extern VALUE make_tobj(gpointer obj, VALUE klass, int size);
extern gpointer get_tobj(VALUE obj, VALUE klass);

#define GDK_BITMAP(b) ((GdkBitmap*)GDK_PIXMAP(b))

#define make_grequisition(c) make_tobj(c, gRequisition, sizeof(GtkRequisition))
#define get_grequisition(c) ((GtkRequisition*)get_tobj(c, gRequisition))

extern VALUE make_gdkregion(GdkRegion* region);
extern GdkRegion* get_gdkregion(VALUE region);

typedef void(*gdkdrawfunc)();

extern VALUE make_gdkevent(GdkEvent* event);
extern GdkEvent* get_gdkevent(VALUE event);

extern VALUE rbgdk_geometry_make(GdkGeometry *geo);
extern GdkGeometry *rbgdk_geometry_get(VALUE geo);

extern VALUE make_gdkatom(GdkAtom atom);
extern GdkAtom get_gdkatom(VALUE atom);

#endif /* _RBGTK_H */
