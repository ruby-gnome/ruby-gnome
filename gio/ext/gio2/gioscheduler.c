/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

static GIOSchedulerJob *
ioschedulerjob_copy(const GIOSchedulerJob *job)
{
        return (GIOSchedulerJob *)job;
}

static void
ioschedulerjob_free(G_GNUC_UNUSED GIOSchedulerJob *job)
{
        return;
}

static GType
g_io_scheduler_job_get_type(void)
{
        static GType our_type = 0;

        if (our_type == 0)
                our_type = g_boxed_type_register_static("GIOSchedulerJob",
                                                        (GBoxedCopyFunc)ioschedulerjob_copy,
                                                        (GBoxedFreeFunc)ioschedulerjob_free);

        return our_type;
}

#define G_TYPE_IO_SCHEDULER_JOB (g_io_scheduler_job_get_type())

#define RVAL2GIOSCHEDULERJOB(object) \
        ((GIOSchedulerJob *)(RVAL2BOXED(object, G_TYPE_IO_SCHEDULER_JOB)))

#define GIOSCHEDULERJOB2RVAL(object) \
        BOXED2RVAL(object, G_TYPE_IO_SCHEDULER_JOB)

#define _SELF(value) RVAL2GIOSCHEDULERJOB(value)

struct ioscheduler_job_callback_data
{
        GIOSchedulerJob *job;
        GCancellable *cancellable;
        gpointer data;
};

static VALUE
ioscheduler_job_callback_call(VALUE data)
{
        static VALUE s_id_call;
        struct ioscheduler_job_callback_data *real;
 
        if (s_id_call == 0)
                s_id_call = rb_intern("call");

        real = (struct ioscheduler_job_callback_data *)data;

        /* TODO: Totally unsure if _UNREF is correct here or not. */
        return rb_funcall(USE_BLOCK_AND_SAVE(real->data), s_id_call, 2,
                          GOBJ2RVAL_UNREF(real->job),
                          GOBJ2RVAL_UNREF(real->cancellable));
}

static gboolean
ioscheduler_job_callback(GIOSchedulerJob *job,
                         GCancellable *cancellable,
                         gpointer data)
{
        struct ioscheduler_job_callback_data real = { job, cancellable, data };

        return RVAL2CBOOL(G_PROTECT_CALLBACK(ioscheduler_job_callback_call,
                                             &real));
}

static void
ioscheduler_job_callback_free(gpointer data)
{
        USE_BLOCK((VALUE)data);
}

static VALUE
ioscheduler_push_job(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        gint io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "01", &rbio_priority, &rbcancellable);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        block = rb_block_proc();
        SAVE_BLOCK(block);
        g_io_scheduler_push_job(ioscheduler_job_callback,
                                (gpointer)block,
                                ioscheduler_job_callback_free,
                                io_priority,
                                cancellable);

        return self;
}

static VALUE
ioscheduler_cancel_all_jobs(VALUE self)
{
        g_io_scheduler_cancel_all_jobs();

        return self;
}

static VALUE
ioscheduler_source_callback_call(VALUE data)
{
        static VALUE s_id_call;
 
        if (s_id_call == 0)
                s_id_call = rb_intern("call");

        return rb_funcall(USE_BLOCK_AND_SAVE(data), s_id_call, 0);
}

static gboolean
ioscheduler_source_callback(gpointer data)
{
        return RVAL2CBOOL(G_PROTECT_CALLBACK(ioscheduler_source_callback_call,
                                             data));
}

static void
ioscheduler_source_callback_free(gpointer data)
{
        USE_BLOCK(data);
}

static VALUE
ioschedulerjob_send_to_mainloop(VALUE self)
{
        VALUE block;

        block = rb_block_proc();
        SAVE_BLOCK(block);

        return CBOOL2RVAL(g_io_scheduler_job_send_to_mainloop(_SELF(self),
                                                              ioscheduler_source_callback,
                                                              (gpointer)block,
                                                              ioscheduler_source_callback_free));
}

static VALUE
ioschedulerjob_send_to_mainloop_async(VALUE self)
{
        VALUE block;

        block = rb_block_proc();
        SAVE_BLOCK(block);

        g_io_scheduler_job_send_to_mainloop_async(_SELF(self),
                                                  ioscheduler_source_callback,
                                                  (gpointer)block,
                                                  ioscheduler_source_callback_free);

        return self;
}

void
Init_gioscheduler(VALUE glib)
{
        VALUE ioschedulerjob = G_DEF_CLASS(G_TYPE_IO_SCHEDULER_JOB, "IOSchedulerJob", glib);

        /* TODO: These don’t follow naming scheme exactly, but seems redundant
         * to add GLib::IOScheduler.  Perhaps rename them to push and
         * cancel_all. */
        rb_define_singleton_method(ioschedulerjob, "push_job", ioscheduler_push_job, -1);
        rb_define_singleton_method(ioschedulerjob, "cancel_all_jobs", ioscheduler_cancel_all_jobs, 0);

        rb_define_method(ioschedulerjob, "send_to_mainloop", ioschedulerjob_send_to_mainloop, 0);
        rb_define_method(ioschedulerjob, "send_to_mainloop_async", ioschedulerjob_send_to_mainloop_async, 0);
}
