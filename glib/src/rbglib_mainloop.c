/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_mainloop.c -

  $Author: mutoh $
  $Date: 2005/11/03 11:53:16 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"
#include <rubysig.h>

/*****************************************/
GType
g_main_loop_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GMainLoop",
                    (GBoxedCopyFunc)g_main_loop_ref,
                    (GBoxedFreeFunc)g_main_loop_unref);
  return our_type;
}
/*****************************************/

#define _SELF(s) ((GMainLoop*)RVAL2BOXED(s, G_TYPE_MAIN_LOOP))

/*****************************************/
static VALUE rbglib_main_threads;

#include <version.h>
#if RUBY_VERSION_CODE >= 154
# define USE_POLL_FUNC 1
#else
# undef USE_POLL_FUNC /* rb_thread_select() may cause busy wait */
#endif

/* We can't use rbglib_poll() on native Win32.
   Because GPollFD doesn't have file descriptor but HANDLE. */
#ifdef G_OS_WIN32
# undef USE_POLL_FUNC
#endif

#ifdef USE_POLL_FUNC

static gint 
rbglib_poll (GPollFD *fds,
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

static gboolean
idle(gpointer data)
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
    g_source_remove(NUM2UINT(data));
}

#endif /* !USE_POLL_FUNC */

/*****************************************/

static VALUE
ml_initialize(self, context, is_running)
    VALUE self, context, is_running;
{
    GMainLoop* ml;
    ml = g_main_loop_new(NIL_P(context)?NULL:RVAL2BOXED(context, G_TYPE_MAIN_CONTEXT), 
                         RTEST(is_running));
    G_INITIALIZE(self, ml);
    return Qnil;
}

/* 
 * An empty timeout 
 */
static gint
empty_timeout_func(gpointer data) { return TRUE; }

static VALUE
ml_run(self)
    VALUE self;
{
    rb_ary_push(rbglib_main_threads, rb_thread_current());

    /* This forces the custom g_poll function to be called 
     * with a minimum timeout of 100ms so that the GMainLoop
     * iterates from time to time even if there is no event.
     * Another way could be to add a wakeup pipe to the selectable
     * fds and than wake up the select only when needed.
     */
    g_timeout_add(100, empty_timeout_func, NULL);

    g_main_loop_run(_SELF(self));
    return self;
}

static VALUE
ml_quit(self)
    VALUE self;
{
    VALUE thread = rb_ary_pop(rbglib_main_threads);

    g_main_loop_quit(_SELF(self));

    if (NIL_P(thread)){
        rb_warning("GLib::MainLoop#quit was called incorrectly.");
    } else {
        rb_thread_wakeup(thread);
    }
    return Qnil;
}

static VALUE
ml_is_running(self)
    VALUE self;
{
    return CBOOL2RVAL(g_main_loop_is_running(_SELF(self)));
}

static VALUE
ml_get_context(self)
    VALUE self;
{
    return BOXED2RVAL(g_main_loop_get_context(_SELF(self)), G_TYPE_MAIN_CONTEXT);
}

void
Init_glib_main_loop()
{
    VALUE ml = G_DEF_CLASS(G_TYPE_MAIN_LOOP, "MainLoop", mGLib); 

    rb_define_method(ml, "initialize", ml_initialize, 2);
    rb_define_method(ml, "run", ml_run, 0);
    rb_define_method(ml, "quit", ml_quit, 0);
    rb_define_method(ml, "running?", ml_is_running, 0);
    rb_define_method(ml, "context", ml_get_context, 0);

    rb_global_variable(&rbglib_main_threads);
    rbglib_main_threads = rb_ary_new();

#ifdef USE_POLL_FUNC
    g_main_context_set_poll_func(NULL, (GPollFunc)rbglib_poll);
#else
    rb_set_end_proc(idle_remove, UINT2NUM(g_idle_add((GSourceFunc)idle, 0)));
#endif
}
