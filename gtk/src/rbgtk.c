/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtk.c -

  $Author: ktou $
  $Date: 2006/04/26 14:37:44 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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

    if (GTK_IS_WINDOW(gtkobj)) {
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

#ifdef USE_POLL_FUNC

static GPollFunc default_poll_func;

static gint 
rbgtk_poll (GPollFD *fds,
            guint    nfds,
            gint     timeout)
{
    struct timeval tv;
    fd_set rset, wset, xset;
    GPollFD *f;
    int ready;
    int maxfd = 0;

    FD_ZERO (&rset);
    FD_ZERO (&wset);
    FD_ZERO (&xset);

    for (f = fds; f < &fds[nfds]; ++f) {
        if (f->fd >= 0)	{
            if (f->events & G_IO_IN)
                FD_SET (f->fd, &rset);
            if (f->events & G_IO_OUT)
                FD_SET (f->fd, &wset);
            if (f->events & G_IO_PRI)
                FD_SET (f->fd, &xset);
            if (f->fd > maxfd && (f->events & (G_IO_IN|G_IO_OUT|G_IO_PRI)))
                maxfd = f->fd;
        }
    }
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    ready = rb_thread_select (maxfd + 1, &rset, &wset, &xset,
                              timeout == -1 ? NULL : &tv);
    if (ready > 0) {
        for (f = fds; f < &fds[nfds]; ++f) {
            f->revents = 0;
            if (f->fd >= 0) {
                if (FD_ISSET (f->fd, &rset))
                    f->revents |= G_IO_IN;
                if (FD_ISSET (f->fd, &wset))
                    f->revents |= G_IO_OUT;
                if (FD_ISSET (f->fd, &xset))
                    f->revents |= G_IO_PRI;
            }
        }
    }

    return ready;
}

static void
restore_poll_func(VALUE data)
{
    if (g_main_context_get_poll_func(NULL) == (GPollFunc)rbgtk_poll) {
        g_main_context_set_poll_func(NULL, default_poll_func);
    }
}

static void
set_internal_poll_func(void)
{
    /* do nothing */
}

static void
remove_internal_poll_func(void)
{
    /* do nothing */
}

#else /* !USE_POLL_FUNC */

static guint idle_id;

static gboolean
idle()
{
    struct timeval wait;

    wait.tv_sec  = 0;
    wait.tv_usec = 10000; /* 10ms */

    CHECK_INTS;
    if (!rb_thread_critical) rb_thread_wait_for(wait);

    return TRUE;
}

static void
idle_remove(VALUE data)
{
    gtk_idle_remove(idle_id);
}

static void
set_internal_poll_func(void)
{
    idle_id = gtk_idle_add((GtkFunction)idle, 0);
}

static void
remove_internal_poll_func(void)
{
    idle_remove(Qnil);
}

#endif /* !USE_POLL_FUNC */


static VALUE
gtk_m_events_pending_internal(self)
    VALUE self;
{
    remove_internal_poll_func();
    return gtk_events_pending() ? Qtrue : Qfalse;
}

static VALUE
gtk_m_events_pending_ensure(self)
    VALUE self;
{
    set_internal_poll_func();
    return Qnil;
}

static VALUE
gtk_m_events_pending(self)
    VALUE self;
{
    return rb_ensure(gtk_m_events_pending_internal, self,
                     gtk_m_events_pending_ensure, self);
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

#ifdef USE_POLL_FUNC
    default_poll_func = g_main_context_get_poll_func(NULL);
    g_main_context_set_poll_func(NULL, (GPollFunc)rbgtk_poll);
    rb_set_end_proc(restore_poll_func, Qnil);
#else
    set_internal_poll_func();
    rb_set_end_proc(idle_remove, Qnil);
#endif

}
