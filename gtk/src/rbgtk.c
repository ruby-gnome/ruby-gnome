/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtk.c -

  $Author: mutoh $
  $Date: 2006/11/17 18:12:41 $

  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2001 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"
#include "st.h"
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
rbgtk_initialize_gtkobject(obj, gtkobj)
    VALUE obj;
    GtkObject *gtkobj;
{
    gtkobj = g_object_ref(gtkobj);
    gtk_object_sink(gtkobj);
    G_INITIALIZE(obj, gtkobj);

    if (GTK_IS_WINDOW(gtkobj) || GTK_IS_MENU_SHELL(gtkobj)) {
        VALUE klass;
        klass = rb_obj_class(obj);
        if (NIL_P(rb_ivar_get(klass, id__windows__))) {
            rb_ivar_set(klass, id__windows__, rb_hash_new());
        }
        rb_hash_aset(rb_ivar_get(klass, id__windows__), obj, Qnil);
        g_signal_connect_after(gtkobj, "destroy",
                               G_CALLBACK(remove_from_windows),
                               (gpointer)obj);
    }
}

void
exec_callback(widget, proc)
    GtkWidget *widget;
    gpointer proc;
{
    rb_funcall((VALUE)proc, id_call, 1, GOBJ2RVAL(widget));
}

/*
 * Gtk module
 */
#include <version.h>
#if RUBY_VERSION_CODE >= 154
# define USE_POLL_FUNC 1
#else
# undef USE_POLL_FUNC /* rb_thread_select() may cause busy wait */
#endif

/* We can't use rbgtk_poll() on native Win32.
   Because GPollFD doesn't have file descriptor but HANDLE. */
#ifdef G_OS_WIN32
# undef USE_POLL_FUNC
#endif

#ifndef USE_POLL_FUNC
static VALUE
gtk_m_events_pending_internal(self)
    VALUE self;
{
    rbg_remove_internal_poll_func((VALUE)Qnil);
    return CBOOL2RVAL(gtk_events_pending());
}

static VALUE
gtk_m_events_pending_ensure(self)
    VALUE self;
{
    rbg_set_internal_poll_func();
    return Qnil;
}
#endif

static VALUE
gtk_m_events_pending(self)
    VALUE self;
{
#ifdef USE_POLL_FUNC
   return CBOOL2RVAL(gtk_events_pending());
#else
   return rb_ensure(gtk_m_events_pending_internal, self,
                    gtk_m_events_pending_ensure, self);
#endif
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
