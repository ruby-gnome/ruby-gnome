/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  utils_callback.c -

  $Author: sakai $
  $Date: 2007/07/14 07:16:18 $

  Copyright (C) 2007  Ruby-GNOME2 Project

**********************************************************************/

#include "rbgprivate.h"

#ifdef G_OS_WIN32
#undef HAVE_NATIVETHREAD /* FIXME */
#endif

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif

#ifndef HAVE_RB_ERRINFO
#define rb_errinfo() (ruby_errinfo)
#endif

#ifndef HAVE_RUBY_NATIVE_THREAD_P
#  define ruby_native_thread_p() is_ruby_native_thread()
#endif

static ID id_exit_application, id_callback_handle_thread;

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
    GMutex *done_mutex;
    GCond *done_cond;
};

static GMutex *pipe_mutex = NULL;
static GMutex *callback_handle_thread_mutex = NULL;
static int callback_fd[2], callback_handle_thread_fd[2];

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
    int max_fd;

    max_fd = MAX(callback_fd[0], callback_handle_thread_fd[0]) + 1;
    for (;;) {
        struct callback_req* req;
        fd_set read_fds;

        /* wait until we're triggered.
         * If this happens we can read from the pipe
         * and it's guaranteed that the needed mutexes are initialized */
        FD_ZERO(&read_fds);
        FD_SET(callback_fd[0], &read_fds);
        FD_SET(callback_handle_thread_fd[0], &read_fds);
        if (rb_thread_select(max_fd, &read_fds, NULL, NULL, NULL) <= 0)
            rb_sys_fail("select() failed in GLib callback handle thread");

        if (FD_ISSET(callback_handle_thread_fd[0], &read_fds))
            break;

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

    return Qnil;
}

static VALUE
invoke_callback_in_ruby_thread(VALUE (*func)(VALUE), VALUE arg)
{
    struct callback_req req;

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
    if (!ruby_native_thread_p()) {
        return invoke_callback_in_ruby_thread(func, arg);
    }
#endif
    return rbgutil_protect(func, arg);
}

/**********************************************************************/

void
rbgutil_start_callback_handle_thread(void)
{
#ifdef HAVE_NATIVETHREAD
    VALUE callback_handle_thread;

    g_mutex_lock(callback_handle_thread_mutex);
    callback_handle_thread = rb_ivar_get(mGLib, id_callback_handle_thread);
    if (NIL_P(callback_handle_thread)) {
        callback_handle_thread = rb_thread_create(mainloop, NULL);
        rb_ivar_set(mGLib, id_callback_handle_thread, callback_handle_thread);
    }
    g_mutex_unlock(callback_handle_thread_mutex);
#endif
}

void
rbgutil_stop_callback_handle_thread(void)
{
#ifdef HAVE_NATIVETHREAD
    g_mutex_lock(callback_handle_thread_mutex);
    if (!NIL_P(rb_ivar_get(mGLib, id_callback_handle_thread))) {
        write(callback_handle_thread_fd[1], "STOP", 4);
        rb_ivar_set(mGLib, id_callback_handle_thread, Qnil);
    }
    g_mutex_unlock(callback_handle_thread_mutex);
#endif
}

void Init_gutil_callback()
{
    id_exit_application = rb_intern("exit_application");
    id_callback_handle_thread = rb_intern("callback_handle_thread");

    rb_ivar_set(mGLib, id_callback_handle_thread, Qnil);

#ifdef HAVE_NATIVETHREAD
    if (!g_thread_supported())
        g_thread_init(NULL);

    pipe_mutex = g_mutex_new();
    callback_handle_thread_mutex = g_mutex_new();

    if (pipe(callback_fd) != 0)
        rb_sys_fail("Unable to create pipe to handle GLib callback "
                    "from other thread");
    if (pipe(callback_handle_thread_fd) != 0)
        rb_sys_fail("Unable to create pipe to stop GLib callback handle thread");
#endif
}
