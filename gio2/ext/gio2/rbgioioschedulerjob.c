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

#define G_TYPE_IO_SCHEDULER_JOB (g_io_scheduler_job_get_type())

#define RVAL2GIOSCHEDULERJOB(object) \
        ((GIOSchedulerJob *)(RVAL2BOXED(object, G_TYPE_IO_SCHEDULER_JOB)))

#define GIOSCHEDULERJOB2RVAL(object) \
        BOXED2RVAL(object, G_TYPE_IO_SCHEDULER_JOB)

#define RG_TARGET_NAMESPACE cIOSchedulerJob
#define _SELF(value) RVAL2GIOSCHEDULERJOB(value)

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
rg_send_to_mainloop(VALUE self)
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
rg_send_to_mainloop_async(VALUE self)
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
Init_gioschedulerjob(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_IO_SCHEDULER_JOB, "IOSchedulerJob", mGio);

        RG_DEF_METHOD(send_to_mainloop, 0);
        RG_DEF_METHOD(send_to_mainloop_async, 0);
}
