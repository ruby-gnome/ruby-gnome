/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_mainloop.c -

  $Author: ktou $
  $Date: 2007/10/03 11:06:01 $

  Copyright (C) 2005,2006 Masao Mutoh
************************************************/

#include "rbgprivate.h"
#include <rubysig.h>
#include <node.h>

#ifdef HAVE_CURR_THREAD
#  define rb_curr_thread curr_thread
#endif

/* from eval.c */
#define WAIT_FD		(1<<0)
#define WAIT_SELECT	(1<<1)
#define WAIT_TIME	(1<<2)
#define WAIT_JOIN	(1<<3)
#define WAIT_PID	(1<<4)

#define DELAY_INFTY 1E30

static double
timeofday()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
}

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
static GPollFunc default_poll_func;

static gint
rg_poll(GPollFD *ufds, guint nfsd, gint timeout)
{
    gint result;
    int thread_critical;

    thread_critical = rb_thread_critical;
    rb_thread_critical = 1;
    TRAP_BEG;
    result = default_poll_func(ufds, nfsd, timeout);
    rb_thread_critical = thread_critical;
    TRAP_END;

    return result;
}

static void
restore_poll_func(VALUE data)
{
    if (g_main_context_get_poll_func(NULL) == (GPollFunc)rg_poll) {
        g_main_context_set_poll_func(NULL, default_poll_func);
    }
}

/*****************************************/
typedef struct _RGSource
{
    GSource source;

    GList *poll_fds;
    gboolean ready;
} RGSource;

static void
source_free_poll_fds(GSource *source, gboolean source_is_destroyed)
{
    RGSource *rg_source = (RGSource *)source;
    GList *node;

    for (node = rg_source->poll_fds; node; node = g_list_next(node)) {
        GPollFD *poll_fd = node->data;
        if (!source_is_destroyed)
            g_source_remove_poll(source, poll_fd);
        g_free(poll_fd);
    }
    g_list_free(rg_source->poll_fds);
    rg_source->poll_fds = NULL;
}

static inline void
source_prepare_add_poll_fd(GSource *source, rb_thread_t thread)
{
    RGSource *rg_source = (RGSource *)source;
    GPollFD *poll_fd;

    poll_fd = g_new0(GPollFD, 1);
    poll_fd->fd = thread->fd;
    poll_fd->events = G_IO_IN;
    if (FD_ISSET(thread->fd, &thread->readfds))
        poll_fd->events |= G_IO_IN | G_IO_PRI;
    if (FD_ISSET(thread->fd, &thread->writefds))
        poll_fd->events |= G_IO_OUT;
    if (FD_ISSET(thread->fd, &thread->exceptfds))
        poll_fd->events |= G_IO_ERR | G_IO_HUP;

    g_source_add_poll(source, poll_fd);
    rg_source->poll_fds = g_list_prepend(rg_source->poll_fds, poll_fd);
}

static inline gboolean
source_prepare_setup_poll_fd(GSource *source, gint *timeout)
{
    rb_thread_t thread;
    gdouble now;

    now = timeofday();
    thread = rb_curr_thread;
    do {
        thread = thread->next;

        if ((thread->wait_for == 0 && thread->status == THREAD_RUNNABLE &&
             thread != rb_curr_thread) ||
            (thread->wait_for & WAIT_JOIN &&
             thread->join->status == THREAD_KILLED))
            return TRUE;

        if (thread->wait_for & WAIT_TIME && thread->delay != DELAY_INFTY) {
            gint delay;

            delay = (thread->delay - now) * 1000;
            if (delay <= 0)
                return TRUE;
            if (*timeout == -1 || delay < *timeout)
                *timeout = delay;
        }

        if (thread->wait_for & WAIT_FD)
            source_prepare_add_poll_fd(source, thread);
    } while (thread != rb_curr_thread);

    return FALSE;
}

static gboolean
source_prepare(GSource *source, gint *timeout)
{
    RGSource *rg_source = (RGSource *)source;

    *timeout = -1;
    source_free_poll_fds(source, FALSE);
    rg_source->ready = source_prepare_setup_poll_fd(source, timeout);

    return rg_source->ready;
}

static gboolean
source_check(GSource *source)
{
    RGSource *rg_source = (RGSource *)source;
    GList *node;

    if (rg_source->ready)
        return TRUE;

    for (node = rg_source->poll_fds; node; node = g_list_next(node)) {
        GPollFD *poll_fd = node->data;

        if (poll_fd->revents)
            return TRUE;
    }

    return FALSE;
}

static gboolean
source_dispatch(GSource *source, GSourceFunc callback, gpointer user_data)
{
    TRAP_BEG;
    rb_thread_schedule();
    TRAP_END;

    return TRUE;
}

static void
source_finalize(GSource *source)
{
    source_free_poll_fds(source, TRUE);
}

static GSourceFuncs source_funcs = {
    source_prepare,
    source_check,
    source_dispatch,
    source_finalize
};

static GSource *
ruby_source_new(void)
{
    GSource *source;
    RGSource *rg_source;

    source = g_source_new(&source_funcs, sizeof(RGSource));
    g_source_set_can_recurse(source, TRUE);

    rg_source = (RGSource *)source;
    rg_source->poll_fds = NULL;

    return source;
}

/*****************************************/

static VALUE
ml_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE context, is_running;
    GMainLoop *loop;
    GMainContext *main_context = NULL;

    rb_scan_args(argc, argv, "02", &context, &is_running);

    if (!NIL_P(context))
        main_context = RVAL2BOXED(context, G_TYPE_MAIN_CONTEXT);
    loop = g_main_loop_new(main_context, RVAL2CBOOL(is_running));
    G_INITIALIZE(self, loop);
    return Qnil;
}

static VALUE
ml_run(self)
    VALUE self;
{
    GMainLoop *loop;
    GSource *source;

    loop = _SELF(self);

    source = ruby_source_new();
    g_source_attach(source, g_main_loop_get_context(loop));
    g_source_unref(source);

    g_main_loop_run(loop);
    return self;
}

static VALUE
ml_quit(self)
    VALUE self;
{
    g_main_loop_quit(_SELF(self));
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
    GSource *source;
    guint tag;

    rb_define_method(ml, "initialize", ml_initialize, -1);
    rb_define_method(ml, "run", ml_run, 0);
    rb_define_method(ml, "quit", ml_quit, 0);
    rb_define_method(ml, "running?", ml_is_running, 0);
    rb_define_method(ml, "context", ml_get_context, 0);

    default_poll_func = g_main_context_get_poll_func(NULL);
    g_main_context_set_poll_func(NULL, rg_poll);
    rb_set_end_proc(restore_poll_func, Qnil);

    source = ruby_source_new();
    tag = g_source_attach(source, NULL);
    g_source_unref(source);
    rb_set_end_proc((void (*)(VALUE))g_source_remove, (VALUE)tag);
}
