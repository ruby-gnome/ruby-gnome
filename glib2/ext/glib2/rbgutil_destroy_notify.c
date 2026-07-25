/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2026  Ruby-GNOME Project Team
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

/*
 * Queue to run GDestroyNotify callbacks in a Ruby thread
 * 
 * Some destroy notify callbacks need to call Ruby APIs,
 * but they may be invoked from a non-Ruby thread.
 * This queue dispatches such callbacks to a Ruby thread
 * where Ruby APIs are available.
 * 
 * Call rbgutil_start_destroy_notify_dispatch_thread() or
 * GObjectIntrospection::Loader.start_destroy_notify_dispatch_thread
 * before queueing requests with rbgutil_queue_destroy_notify_request().
 * 
 * The dispatch thread is alive for the Ruby VM lifetime
 * in order to avoids problems from the order of GC,
 * Ruby VM termination and destroy notify callbacks.
 * 
 * See also rbgutil_callback.c.
 */

#include "rbgprivate.h"
#include "rbgutil.h"
#include "rbgutil_list.h"

typedef struct _DestroyNotifyRequest {
    GDestroyNotify destroy_notify;
    gpointer data;
} DestroyNotifyRequest;

static GMutex destroy_notify_dispatch_thread_mutex;
static GAsyncQueue *destroy_notify_dispatch_queue = NULL;
static ID id_destroy_notify_dispatch_thread;
static gint destroy_notify_pipe_fds[2] = {-1, -1};
static gint destroy_notify_dispatch_pending = 0;

#define DESTROY_NOTIFY_PIPE_READY_MESSAGE "R"
#define DESTROY_NOTIFY_PIPE_READY_MESSAGE_SIZE 1

static VALUE
process_request(VALUE user_data)
{
    DestroyNotifyRequest *request = (DestroyNotifyRequest *)user_data;

    request->destroy_notify(request->data);

    return Qnil;
}

static VALUE
mainloop(void *user_data) {
    for (;;) {
        gchar ready_message_buffer[DESTROY_NOTIFY_PIPE_READY_MESSAGE_SIZE];

        rb_thread_wait_fd(destroy_notify_pipe_fds[0]);
        if (read(destroy_notify_pipe_fds[0],
                 ready_message_buffer,
                 DESTROY_NOTIFY_PIPE_READY_MESSAGE_SIZE
                ) != DESTROY_NOTIFY_PIPE_READY_MESSAGE_SIZE ||
            strncmp(ready_message_buffer,
                    DESTROY_NOTIFY_PIPE_READY_MESSAGE,
                    DESTROY_NOTIFY_PIPE_READY_MESSAGE_SIZE) != 0) {
            g_error("failed to read valid callback dispatcher message");
            continue;
        }
        for (;;) {
            for (;;) {
                DestroyNotifyRequest *request;

                request = g_async_queue_try_pop(destroy_notify_dispatch_queue);
                if (!request) {
                    break;
                }

                rbgutil_protect(process_request, (VALUE)request);
                g_free(request);
            }

            g_atomic_int_set(&destroy_notify_dispatch_pending, 0);

            if (g_async_queue_length(destroy_notify_dispatch_queue) == 0) {
                break;
            } else {
                g_atomic_int_set(&destroy_notify_dispatch_pending, 1);
            }
        }
    }

    return Qnil;
}

void
rbgutil_queue_destroy_notify_request(GDestroyNotify destroy_notify, gpointer data)
{
    DestroyNotifyRequest *request;
    ssize_t written;

    request = g_new(DestroyNotifyRequest, 1);
    request->destroy_notify = destroy_notify;
    request->data = data;

    g_async_queue_push(destroy_notify_dispatch_queue, request);
    if (g_atomic_int_compare_and_exchange(&destroy_notify_dispatch_pending, 0, 1)) {
        written = write(destroy_notify_pipe_fds[1],
                        DESTROY_NOTIFY_PIPE_READY_MESSAGE,
                        DESTROY_NOTIFY_PIPE_READY_MESSAGE_SIZE);
        if (written != DESTROY_NOTIFY_PIPE_READY_MESSAGE_SIZE) {
            g_warning("couldn't write all destroy notify pipe ready message: "
                    "message-size: %d, written: %" G_GSSIZE_FORMAT,
                    DESTROY_NOTIFY_PIPE_READY_MESSAGE_SIZE,
                    written);
        }
    }
}

void
rbgutil_start_destroy_notify_dispatch_thread(void)
{
    VALUE destroy_notify_dispatch_thread;

    g_mutex_lock(&destroy_notify_dispatch_thread_mutex);
    destroy_notify_dispatch_thread = rb_ivar_get(rbg_mGLib(), id_destroy_notify_dispatch_thread);
    if (NIL_P(destroy_notify_dispatch_thread)) {
        if (pipe(destroy_notify_pipe_fds) == -1) {
            rb_sys_fail("pipe()");
        }
        destroy_notify_dispatch_thread = rb_thread_create(mainloop, NULL);
        rb_ivar_set(rbg_mGLib(), id_destroy_notify_dispatch_thread, destroy_notify_dispatch_thread);
    }
    g_mutex_unlock(&destroy_notify_dispatch_thread_mutex);
}

void
Init_gutil_destroy_notify(void)
{
    id_destroy_notify_dispatch_thread = rb_intern("destroy_notify_dispatch_thread");
    rb_ivar_set(rbg_mGLib(), id_destroy_notify_dispatch_thread, Qnil);

    destroy_notify_dispatch_queue = g_async_queue_new();
    g_mutex_init(&destroy_notify_dispatch_thread_mutex);
}
