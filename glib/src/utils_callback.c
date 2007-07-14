/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  utils_callback.c -

  $Author: sakai $
  $Date: 2007/07/14 07:16:18 $

  Copyright (C) 2007  Ruby-GNOME2 Project

**********************************************************************/

#include "global.h"

#ifdef G_OS_WIN32
#undef HAVE_NATIVETHREAD /* FIXME */
#endif

#ifdef HAVE_NATIVETHREAD
#include <unistd.h>
#endif

#ifndef HAVE_RB_ERRINFO
#define rb_errinfo() (ruby_errinfo)
#endif

/* FIXME: ruby 1.9's is_ruby_native_thread() always returnes Qtrue... */
#ifdef HAVE_RB_THREAD_BLOCKING_REGION
#define is_ruby_native_thread() (!g_thread_supported())
#endif

static gboolean callback_initialized = FALSE;
static ID id_exit_application;

/**********************************************************************/

VALUE
rbgutil_protect(VALUE (*func) (VALUE), VALUE data)
{
    int state = 0;
    VALUE ret = rb_protect(func, data, &state);
    VALUE e = rb_errinfo();
    if (state && !NIL_P(e))
        rb_funcall(mGLib, id_exit_application, 1, EXIT_FAILURE);
    return ret;
}

/**********************************************************************/

#ifdef HAVE_NATIVETHREAD

struct callback_req {
    VALUE (*func)(VALUE);
    VALUE arg;
    VALUE ret;
    GMutex* done_mutex;
    GCond* done_cond;
};

static GMutex* pipe_mutex = NULL;
static int callback_fd[2];

static VALUE
exec_callback(VALUE v)
{
    struct callback_req* req = (struct callback_req*)v;
    return req->func(req->arg);
}

static VALUE
process_req(struct callback_req* req)
{
    req->ret = rbgutil_protect(exec_callback, (VALUE)req);
    if (req->done_cond) {
        g_mutex_lock(req->done_mutex);
        g_cond_signal(req->done_cond);
        g_mutex_unlock(req->done_mutex);
    }
    return Qnil;
}

static VALUE
mainloop(void)
{
    for (;;) {
        struct callback_req* req;

        /* wait untill we're triggered.
         * If this happens we can read from the pipe
         * and it's guaranteed that the needed mutexes are initialized */
        rb_thread_wait_fd(callback_fd[0]);

        {
            ssize_t size;
            g_mutex_lock(pipe_mutex); /* lock pipe_mutex to read atomically */
            size = read(callback_fd[0], &req, sizeof(req));
            g_mutex_unlock(pipe_mutex);
            /* FIXME: check size */
        }

        /* To make it reentrant we need to create a new thread. */
        /* It may be nice to use thread pool. */
        rb_thread_create(process_req, req);
        rb_thread_schedule();
    }
}

static VALUE
invoke_callback_in_ruby_thread(VALUE (*func)(VALUE), VALUE arg)
{
    struct callback_req req;

    if (!g_thread_supported())
        rb_bug("glib callback in another thread, but gthreads not supported");

    /* initialize mutex */
    /* FIXME: use g_once? */
    if (!pipe_mutex) {
        static GStaticMutex init_mutex = G_STATIC_MUTEX_INIT;
        g_static_mutex_lock(&init_mutex);
        if (!pipe_mutex)
            pipe_mutex = g_mutex_new();
        g_static_mutex_unlock(&init_mutex);
    }

    req.func = func;
    req.arg  = arg;
    req.ret  = Qnil;
    req.done_mutex = g_mutex_new();
    req.done_cond  = g_cond_new();

    g_mutex_lock(req.done_mutex);

    /* trigger ruby callback thread */
    {
        struct callback_req* p = &req;
        ssize_t size;

        g_mutex_lock(pipe_mutex); /* lock pipe_mutex to write atomically */
        size = write(callback_fd[1], &p, sizeof(p));
        g_mutex_unlock(pipe_mutex);
        /* FIXME: check size */
    }

    /* waiting callback to end */
    g_cond_wait(req.done_cond, req.done_mutex);
    g_mutex_unlock(req.done_mutex);

    g_cond_free(req.done_cond);
    g_mutex_free(req.done_mutex);

    return req.ret;
}

#endif

/**********************************************************************/

VALUE
rbgutil_invoke_callback(VALUE (*func)(VALUE), VALUE arg)
{
#ifdef HAVE_NATIVETHREAD
    if (!is_ruby_native_thread()) {
        return invoke_callback_in_ruby_thread(func, arg);
    }
#endif
    return rbgutil_protect(func, arg);
}

/**********************************************************************/

static void
callback_end_proc(VALUE _)
{
    callback_initialized = FALSE;
}

void Init_utils_callback()
{
    id_exit_application = rb_intern("exit_application");
    callback_initialized = TRUE;
    rb_set_end_proc(callback_end_proc, Qnil);

#ifdef HAVE_NATIVETHREAD
    /* startup the ruby thread to pull callbacks from other threads */
    {
        static VALUE thread;
        if (pipe(callback_fd) != 0)
            rb_bug("Unable to create glib callback thread\n");
        thread = rb_thread_create(mainloop, NULL);
        rb_global_variable(&thread);
    }
#endif
}
