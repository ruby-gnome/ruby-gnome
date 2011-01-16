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

#define _SELF(value) G_IO_STREAM(RVAL2GOBJ(value))

static VALUE
iostream_close(VALUE self, VALUE cancellable)
{
        GError *error = NULL;

        if (!g_io_stream_close(_SELF(self), RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
iostream_close_async(int argc, VALUE *argv, VALUE self)
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
iostream_close_finish(VALUE self, VALUE result)
{
        GError *error = NULL;

        if (!g_io_stream_close_finish(_SELF(self),
                                      RVAL2GASYNCRESULT(result),
                                      &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
iostream_has_pending(VALUE self)
{
        return CBOOL2RVAL(g_io_stream_has_pending(_SELF(self)));
}

static VALUE
iostream_set_pending(VALUE self)
{
        GError *error = NULL;

        if (!g_io_stream_set_pending(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
iostream_clear_pending(VALUE self)
{
        g_io_stream_clear_pending(_SELF(self));

        return self;
}

void
Init_giostream(VALUE glib)
{
        VALUE iostream = G_DEF_CLASS(G_TYPE_IO_STREAM, "IOStream", glib);

        rb_undef_alloc_func(iostream);

        rb_define_method(iostream, "close", iostream_close, 1);
        rb_define_method(iostream, "close_async", iostream_close_async, -1);
        rb_define_method(iostream, "close_finish", iostream_close_finish, 1);
        rb_define_method(iostream, "has_pending?", iostream_has_pending, 0);
        rb_define_method(iostream, "set_pending", iostream_set_pending, 0);
        G_DEF_SETTER(iostream, "pending");
        rb_define_method(iostream, "clear_pending", iostream_clear_pending, 0);
}
