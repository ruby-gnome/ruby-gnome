/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2007-2026  Ruby-GNOME Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgprivate.h"

#ifdef G_OS_WIN32
#  ifdef HAVE_IO_H
#    include <io.h>
#    define pipe(phandles) _pipe(phandles, 128, _O_BINARY)
#  endif
#else
#  ifdef HAVE_UNISTD_H
#    include <unistd.h>
#  endif
#endif
#include <fcntl.h>
#include <errno.h>

static VALUE rbgutil_eGLibCallbackNotInitializedError;
static ID id_exit_application;

/**********************************************************************/

VALUE
rbgutil_protect(VALUE (*func) (VALUE), VALUE data)
{
    int state = 0;
    VALUE ret = rb_protect(func, data, &state);
    VALUE e = rb_errinfo();
    if (state && !NIL_P(e))
        rbgutil_on_callback_error(e);
    return ret;
}

void
rbgutil_on_callback_error(VALUE error)
{
    rb_funcall(rbg_mGLib(), id_exit_application, 2, error, INT2NUM(EXIT_FAILURE));
}

/**********************************************************************/

typedef struct _CallbackRequest {
    VALUE (*function)(VALUE);
    VALUE argument;
    VALUE result;
    /* An async request is heap-allocated by the requester, doesn't
     * report its result and is freed by the dispatch thread. done_mutex
     * and done_cond are unused for it. */
    gboolean async;
    GMutex done_mutex;
    GCond done_cond;
} CallbackRequest;

static GMutex callback_dispatch_thread_mutex;
static GAsyncQueue *callback_request_queue = NULL;
static ID id_callback_dispatch_thread;
static gint callback_pipe_fds[2] = {-1, -1};

#define CALLBACK_PIPE_READY_MESSAGE "R"
#define CALLBACK_PIPE_READY_MESSAGE_SIZE 1

static CallbackRequest *
callback_request_new_async(VALUE (*func)(VALUE), VALUE arg)
{
    CallbackRequest *request;

    request = g_new0(CallbackRequest, 1);
    request->function = func;
    request->argument = arg;
    request->result = Qnil;
    request->async = TRUE;
    return request;
}

static void
callback_request_free(CallbackRequest *request)
{
    g_free(request);
}

static VALUE
exec_callback(VALUE data)
{
    CallbackRequest *request = (CallbackRequest *)data;
    return request->function(request->argument);
}

static VALUE
process_request(void *user_data)
{
    CallbackRequest *request = user_data;
    g_mutex_lock(&(request->done_mutex));
    request->result = rbgutil_protect(exec_callback, (VALUE)request);
    g_cond_signal(&(request->done_cond));
    g_mutex_unlock(&(request->done_mutex));

    return Qnil;
}

static VALUE
mainloop(G_GNUC_UNUSED void *user_data)
{
    for (;;) {
        CallbackRequest *request;
        gchar ready_message_buffer[CALLBACK_PIPE_READY_MESSAGE_SIZE];

        rb_thread_wait_fd(callback_pipe_fds[0]);
        if (read(callback_pipe_fds[0], ready_message_buffer,
                 CALLBACK_PIPE_READY_MESSAGE_SIZE
                ) != CALLBACK_PIPE_READY_MESSAGE_SIZE ||
            strncmp(ready_message_buffer,
                    CALLBACK_PIPE_READY_MESSAGE,
                    CALLBACK_PIPE_READY_MESSAGE_SIZE) != 0) {
            g_error("failed to read valid callback dispatcher message");
            continue;
        }
        request = g_async_queue_pop(callback_request_queue);
        if (!request)
            break;

        if (request->async) {
            /* Async callbacks must be fast because they block the
             * dispatch thread. */
            rbgutil_protect(exec_callback, (VALUE)request);
            callback_request_free(request);
        } else {
            rb_thread_create(process_request, request);
        }
    }

    close(callback_pipe_fds[0]);
    callback_pipe_fds[0] = -1;
    close(callback_pipe_fds[1]);
    callback_pipe_fds[1] = -1;

    return Qnil;
}

static void
queue_callback_request(CallbackRequest *request)
{
    ssize_t written;

    g_async_queue_push(callback_request_queue, request);
    written = write(callback_pipe_fds[1],
                    CALLBACK_PIPE_READY_MESSAGE,
                    CALLBACK_PIPE_READY_MESSAGE_SIZE);
    if (written != CALLBACK_PIPE_READY_MESSAGE_SIZE) {
        rb_warn("couldn't write all callback pipe ready message: "
                "message-size: %d, written: %" G_GSSIZE_FORMAT,
                CALLBACK_PIPE_READY_MESSAGE_SIZE,
                written);
    }
}

static VALUE
invoke_callback_in_ruby_thread(VALUE (*func)(VALUE), VALUE arg)
{
    CallbackRequest request;

    g_mutex_lock(&callback_dispatch_thread_mutex);
    if (callback_pipe_fds[0] == -1) {
        g_error("Please call rbgutil_start_callback_dispatch_thread() "
                "to dispatch a callback from non-Ruby thread before "
                "callbacks are requested from non-Ruby thread.");
        g_mutex_unlock(&callback_dispatch_thread_mutex);
        return Qnil;
    }

    request.function = func;
    request.argument = arg;
    request.result = Qnil;
    request.async = FALSE;
    g_mutex_init(&(request.done_mutex));
    g_cond_init(&(request.done_cond));

    g_mutex_lock(&(request.done_mutex));
    queue_callback_request(&request);
    g_mutex_unlock(&callback_dispatch_thread_mutex);

    g_cond_wait(&(request.done_cond), &(request.done_mutex));
    g_mutex_unlock(&(request.done_mutex));

    g_cond_clear(&(request.done_cond));
    g_mutex_clear(&(request.done_mutex));


    return request.result;
}

static void
invoke_callback_async_in_ruby_thread(VALUE (*func)(VALUE), VALUE arg)
{
    g_mutex_lock(&callback_dispatch_thread_mutex);
    if (callback_pipe_fds[0] == -1) {
        g_mutex_unlock(&callback_dispatch_thread_mutex);
        g_error("Please call rbgutil_start_callback_dispatch_thread() "
                "to dispatch a callback from non-Ruby thread before "
                "callbacks are requested from non-Ruby thread.");
        return;
    }

    queue_callback_request(callback_request_new_async(func, arg));
    g_mutex_unlock(&callback_dispatch_thread_mutex);
}

static void *
invoke_callback_with_gvl(void *arg)
{
    CallbackRequest *req = (CallbackRequest*)arg;
    return (void *)rbgutil_protect(req->function, req->argument);
}

/**********************************************************************/

VALUE
rbgutil_invoke_callback(VALUE (*func)(VALUE), VALUE arg)
{
    if (ruby_native_thread_p()) {
        if (GPOINTER_TO_INT(g_private_get(&rg_polling_key))) {
            CallbackRequest req;
            req.function = func;
            req.argument = arg;
            return (VALUE)rb_thread_call_with_gvl(invoke_callback_with_gvl, &req);
        } else {
            return rbgutil_protect(func, arg);
        }
    } else {
        return invoke_callback_in_ruby_thread(func, arg);
    }
}

/*
 * Fire-and-forget variant of rbgutil_invoke_callback().
 *
 * When this is called from a non-Ruby thread, this doesn't wait for
 * the callback result; the callback is run later in the callback
 * dispatch thread. Use this for callbacks that are invoked frequently
 * from non-Ruby threads and don't need to report a result, such as
 * GDestroyNotify. See also rbgutil_invoke_callback().
 */
void
rbgutil_invoke_callback_async(VALUE (*func)(VALUE), VALUE arg)
{
    if (ruby_native_thread_p()) {
        if (GPOINTER_TO_INT(g_private_get(&rg_polling_key))) {
            CallbackRequest request;
            request.function = func;
            request.argument = arg;
            rb_thread_call_with_gvl(invoke_callback_with_gvl, &request);
        } else {
            rbgutil_protect(func, arg);
        }
    } else {
      invoke_callback_async_in_ruby_thread(func, arg);
    }
}

/**********************************************************************/

void
rbgutil_start_callback_dispatch_thread(void)
{
    VALUE callback_dispatch_thread;

    g_mutex_lock(&callback_dispatch_thread_mutex);
    callback_dispatch_thread = rb_ivar_get(rbg_mGLib(), id_callback_dispatch_thread);
    if (NIL_P(callback_dispatch_thread)) {
        if (pipe(callback_pipe_fds) == -1)
            rb_sys_fail("pipe()");

        callback_dispatch_thread = rb_thread_create(mainloop, NULL);
        rb_ivar_set(rbg_mGLib(), id_callback_dispatch_thread,
                    callback_dispatch_thread);
    }
    g_mutex_unlock(&callback_dispatch_thread_mutex);
}

void
rbgutil_stop_callback_dispatch_thread(void)
{
    VALUE callback_dispatch_thread;

    g_mutex_lock(&callback_dispatch_thread_mutex);
    callback_dispatch_thread = rb_ivar_get(rbg_mGLib(), id_callback_dispatch_thread);
    if (!NIL_P(callback_dispatch_thread)) {
        queue_callback_request(NULL);
        rb_ivar_set(rbg_mGLib(), id_callback_dispatch_thread, Qnil);
    }
    g_mutex_unlock(&callback_dispatch_thread_mutex);
}

void
Init_gutil_callback(void)
{
    id_exit_application = rb_intern("exit_application");
    rbgutil_eGLibCallbackNotInitializedError =
        rb_define_class_under(rbg_mGLib(), "CallbackNotInitializedError",
                              rb_eRuntimeError);

    id_callback_dispatch_thread = rb_intern("callback_dispatch_thread");
    rb_ivar_set(rbg_mGLib(), id_callback_dispatch_thread, Qnil);

    callback_request_queue = g_async_queue_new();
    g_mutex_init(&callback_dispatch_thread_mutex);
}
