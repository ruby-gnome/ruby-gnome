/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtk.c -

  $Author: sakai $
  $Date: 2003/02/14 18:58:19 $

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

void
rbgtk_initialize_gtkobject(obj, gtkobj)
    VALUE obj;
    GtkObject *gtkobj;
{
    gtkobj = g_object_ref(gtkobj);
    gtk_object_sink(gtkobj);
    G_INITIALIZE(obj, gtkobj);
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

#ifdef HAVE_SYS_SELECT_H
# include <sys/select.h>
#endif

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

#else /* !USE_POLL_FUNC */

static gint
idle()
{
    struct timeval wait;

    wait.tv_sec  = 0;
    wait.tv_usec = 100000; /* 100ms */

    CHECK_INTS;
    if (!rb_thread_critical) rb_thread_wait_for(wait);

    return Qtrue;
}

static void
idle_remove(VALUE data)
{
    gtk_idle_remove(NUM2UINT(data));
}

#endif /* !USE_POLL_FUNC */


/*
 * Init
 */
void 
Init_gtk_gtk()
{
    id_call = rb_intern("call");

    mGtk = rb_define_module("Gtk");
    rb_ivar_set(mGtk, id_relative_callbacks, Qnil);

#ifdef USE_POLL_FUNC
    g_main_set_poll_func(rbgtk_poll);
#else
    rb_set_end_proc(idle_remove, UINT2NUM(gtk_idle_add((GtkFunction)idle, 0)));
#endif

}
