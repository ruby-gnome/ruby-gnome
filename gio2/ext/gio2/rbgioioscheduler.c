/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE mIOScheduler

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
rg_s_push_job(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        gint io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "02", &rbio_priority, &rbcancellable);
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
rg_s_cancel_all_jobs(VALUE self)
{
        g_io_scheduler_cancel_all_jobs();

        return self;
}

void
Init_gioscheduler(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGio, "IOScheduler");
        RG_DEF_SMETHOD(push_job, -1);
        RG_DEF_SMETHOD(cancel_all_jobs, 0);
}
