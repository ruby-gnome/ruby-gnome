/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtk.h -

  $Author: sakai $
  $Date: 2003/02/17 11:29:20 $

  Copyright (C) 2003 Ruby-GNOME2 Project Team
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
#define RVAL2ATOM(atom) (get_gdkatom(atom))

#define RBGTK_INITIALIZE(obj,gtkobj)\
 (rbgtk_initialize_gtkobject(obj, GTK_OBJECT(gtkobj)))

#define GDK_TYPE_GEOMETRY (gdk_geometry_get_type())
#define GDK_TYPE_REGION (gdk_region_get_type())
#define GDK_TYPE_SPAN (gdk_span_get_type())
#define GDK_TYPE_ATOM (gdk_atom_get_type())
#define GTK_TYPE_ACCEL_KEY (gtk_accel_key_get_type())
#define GTK_TYPE_ACCEL_GROUP_ENTRY (gtk_accel_group_entry_get_type())

extern VALUE mGtk;
extern VALUE mGdk;

/*
 * for gtk2.0/gtk2.2
 */
#define RBGTK_TYPE_CLIPBOARD (rbgtk_clipboard_get_type())
#define RVAL2CLIPBOARD(obj) rbgtk_get_clipboard(obj)
extern GType (*rbgtk_clipboard_get_type)();
extern GtkClipboard* (*rbgtk_get_clipboard)(VALUE obj);

#define RBGTK_TYPE_TREE_ROW_REFERENCE (rbgtk_tree_row_reference_get_type())
#define RVAL2TREEROWREFERENCE(obj) (rbgtk_get_tree_row_reference(obj))
extern GType (*rbgtk_tree_row_reference_get_type)();
extern GtkTreeRowReference *(*rbgtk_get_tree_row_reference)(VALUE obj);

/*
 * for gtk
 */
extern void rbgtk_initialize_gtkobject(VALUE obj, GtkObject *gtkobj);
extern void exec_callback(GtkWidget *widget, gpointer proc);
extern GType gtk_accel_key_get_type();
extern GType gtk_accel_group_entry_get_type();

/*
 * Gtk::TreeIter callback
 */
typedef void (*rbgtkiter_set_value_func)(void *model, GtkTreeIter *iter,
                                         gint column, GValue *value);
extern void rbgtk_register_treeiter_set_value_func(VALUE klass, rbgtkiter_set_value_func);
extern VALUE treeiter_set_value_table;

/*
 * Gtk::SelectionData
 */
extern void rbgtk_atom2selectiondata(VALUE type, VALUE size, VALUE src, GdkAtom* gtype,
                                     void** data, gint* format, gint* length);
extern void rbgtk_atom2selectiondata_free(GdkAtom gtype, void* data); 

/*
 * Gtk::Drag
 */
extern GtkTargetEntry* rbgtk_get_target_entry(VALUE targets);

/*
 * for gdk
 */
typedef struct {
  GdkAtom atom;
} GdkAtomData;

extern GType gdk_atom_get_type(void);
extern GType gdk_geometry_get_type(void);
extern GType gdk_region_get_type(void);
extern GType gdk_span_get_type(void);

extern GdkAtom get_gdkatom(VALUE atom);
extern VALUE make_gdkevent(GdkEvent* event);
extern GdkEvent* get_gdkevent(VALUE event);


#endif /* _RBGTK_H */
