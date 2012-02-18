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

#define RG_TARGET_NAMESPACE cIOStream
#define _SELF(value) RVAL2GIOSTREAM(value)

static VALUE
rg_close(VALUE self, VALUE cancellable)
{
        GError *error = NULL;

        if (!g_io_stream_close(_SELF(self), RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_close_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "01&", &rbio_priority, &rbcancellable, &block);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_io_stream_close_async(_SELF(self),
                                io_priority,
                                cancellable,
                                rbgio_async_ready_callback,
                                (gpointer)block);

        return self;
}

static VALUE
rg_close_finish(VALUE self, VALUE result)
{
        GError *error = NULL;

        if (!g_io_stream_close_finish(_SELF(self),
                                      RVAL2GASYNCRESULT(result),
                                      &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_has_pending_p(VALUE self)
{
        return CBOOL2RVAL(g_io_stream_has_pending(_SELF(self)));
}

static VALUE
rg_set_pending(VALUE self)
{
        GError *error = NULL;

        if (!g_io_stream_set_pending(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_clear_pending(VALUE self)
{
        g_io_stream_clear_pending(_SELF(self));

        return self;
}

void
Init_giostream(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_IO_STREAM, "IOStream", mGio);

        RG_DEF_METHOD(close, 1);
        RG_DEF_METHOD(close_async, -1);
        RG_DEF_METHOD(close_finish, 1);
        RG_DEF_METHOD_P(has_pending, 0);
        RG_DEF_METHOD(set_pending, 0);
        RG_DEF_METHOD(clear_pending, 0);
}
