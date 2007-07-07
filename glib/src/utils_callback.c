/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  utils_callback.c -

  $Author: sakai $
  $Date: 2007/07/07 14:03:35 $

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

struct callback_info {
    VALUE (*func)(VALUE);
    VALUE arg;
    VALUE ret;
    GMutex* done_mutex;
    GCond* done_cond;
};

static GStaticMutex init_mutex = G_STATIC_MUTEX_INIT;
static GMutex* pipe_mutex = NULL;
static int callback_fd[2];

static VALUE
callback_info_run_body(VALUE v)
{
    struct callback_info* pinfo = (struct callback_info*)v;
    pinfo->func(pinfo->arg);
    return Qnil;
}

static VALUE
callback_info_run(struct callback_info* pinfo)
{
    int state = 0;
    pinfo->ret = rb_protect(callback_info_run_body, (VALUE)pinfo, &state);
    if (pinfo->done_cond) {
        g_mutex_lock(pinfo->done_mutex);
        g_cond_signal(pinfo->done_cond);
        g_mutex_unlock(pinfo->done_mutex);
    }
    return Qnil;
}

static VALUE
listen_callback_pipe(void)
{
    struct callback_info* info;

    for (;;) {
        ssize_t size;

        /* wait untill we're triggered.
         * If this happens we can read from the pipe
         * and it's guaranteed that the needed mutexes are initialized */
        rb_thread_wait_fd(callback_fd[0]);

        /* lock pipe_mutex to read atomically */
        g_mutex_lock(pipe_mutex);
        size = read(callback_fd[0], &info, sizeof(info));
        g_mutex_unlock(pipe_mutex);

        /* To make it reentrant we need to create a new thread. */
        /* It may be nice to use thread pool. */
        rb_thread_create(callback_info_run, info);
    }
}

static VALUE
invoke_callback_in_ruby_thread(VALUE (*func)(VALUE), VALUE arg)
{
    struct callback_info info;
    struct callback_info* pinfo;
    ssize_t size;

    if (!g_thread_supported())
        rb_bug("glib callback in another thread, but gthreads not supported");

    /* initialize mutex */
    if (!pipe_mutex) {
        g_static_mutex_lock(&init_mutex);
        if (!pipe_mutex) {
            pipe_mutex = g_mutex_new();
        }
        g_static_mutex_unlock(&init_mutex);
    }

    info.func = func;
    info.arg  = arg;
    info.ret  = Qnil;
    info.done_mutex = g_mutex_new();
    info.done_cond  = g_cond_new();
    pinfo = &info;

    /* trigger ruby callback thread */
    /* lock pipe_mutex to write atomically */
    g_mutex_lock(pipe_mutex);
    size = write(callback_fd[1], &pinfo, sizeof(pinfo));
    g_mutex_unlock(pipe_mutex);

    /* waiting callback to end */
    g_mutex_lock(info.done_mutex);
    g_cond_wait(info.done_cond, info.done_mutex);
    g_mutex_unlock(info.done_mutex);

    g_cond_free(info.done_cond);
    g_mutex_free(info.done_mutex);

    return info.ret;
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
        thread = rb_thread_create(listen_callback_pipe, NULL);
        rb_global_variable(&thread);
    }
#endif
}
