/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtk.c -

  $Author: sakai $
  $Date: 2007/07/04 13:33:17 $

  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2001 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

VALUE mGtk;

ID id_relative_callbacks;
ID id_call;
static ID id__windows__;

static void
remove_from_windows(GtkWidget *window, VALUE obj)
{
    VALUE klass;
    klass = rb_obj_class(obj);
    rb_hash_delete(rb_ivar_get(klass, id__windows__), obj);
}

void
rbgtk_initialize_gtkobject(VALUE obj, GtkObject *gtkobj)
{
    gtkobj = g_object_ref(gtkobj);
    gtk_object_sink(gtkobj);
    G_INITIALIZE(obj, gtkobj);

    if (GTK_IS_WINDOW(gtkobj) || GTK_IS_MENU_SHELL(gtkobj)) {
        VALUE klass;
        klass = rb_obj_class(obj);
        if (rb_ivar_defined(klass, id__windows__) == Qfalse) {
            rb_ivar_set(klass, id__windows__, rb_hash_new());
        }
        rb_hash_aset(rb_ivar_get(klass, id__windows__), obj, Qnil);
        g_signal_connect_after(gtkobj, "destroy",
                               G_CALLBACK(remove_from_windows),
                               (gpointer)obj);
    }
}

void
exec_callback(GtkWidget *widget, gpointer proc)
{
    rb_funcall((VALUE)proc, id_call, 1, GOBJ2RVAL(widget));
}

/*
 * Gtk module
 */

/* We can't use rbgtk_poll() on native Win32.
   Because GPollFD doesn't have file descriptor but HANDLE. */
#define USE_POLL_FUNC

static VALUE
gtk_m_events_pending(VALUE self)
{
   return CBOOL2RVAL(gtk_events_pending());
}

/*
 * Init
 */
void 
Init_gtk_gtk()
{
    id_call = rb_intern("call");
    id__windows__ = rb_intern("__windows__");

    mGtk = rb_define_module("Gtk");
    rb_ivar_set(mGtk, id_relative_callbacks, Qnil);

    rb_define_module_function(mGtk, "events_pending?", gtk_m_events_pending, 0);

}
