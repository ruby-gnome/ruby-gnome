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

#define _SELF(value) \
        G_BUFFERED_INPUT_STREAM(RVAL2GOBJ(value))

DECLARE_VARG(initialize)
{
        VALUE rbbase_stream,
              size;
        GInputStream *base_stream;
        GInputStream *stream;

        rb_scan_args(argc, argv, "11", &rbbase_stream, &size);
        base_stream = RVAL2GINPUTSTREAM(rbbase_stream);

        if (NIL_P(size))
                stream = g_buffered_input_stream_new(base_stream);
        else
                stream = g_buffered_input_stream_new_sized(base_stream,
                                                           NUM2UINT(size));

        G_INITIALIZE(self, stream);

        return Qnil;
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self) \
        { \
                return type(g_buffered_input_stream_get_##name(_SELF(self))); \
        }

DEFGETMETHOD(buffer_size, UINT2NUM)

DECLARE(set_buffer_size)(VALUE self, VALUE size)
{
        g_buffered_input_stream_set_buffer_size(_SELF(self),
                                                NUM2UINT(size));

        return self;
}

DEFGETMETHOD(available, UINT2NUM)

DECLARE(peek_buffer)(VALUE self)
{
        gsize size;
        const void *buffer;

        buffer = g_buffered_input_stream_peek_buffer(_SELF(self), &size);

        return CSTR2RVAL_OWN_FROZEN((const char *)buffer, size);
}

DECLARE(peek)(VALUE self, VALUE rboffset, VALUE rbcount)
{
        gsize offset = NUM2UINT(rboffset);
        gsize count = NUM2UINT(rbcount);
        VALUE buffer = rb_str_new(NULL, count);
        gsize bytes_peeked = g_buffered_input_stream_peek(_SELF(self),
                                                          RSTRING_PTR(buffer),
                                                          offset,
                                                          count);
        if (bytes_peeked == (gsize)-1)
                rb_raise(rb_eRuntimeError, "Cannot peek"); /* TODO: Better class and message */

        return buffer;
}

DECLARE_VARG(fill)
{
        gssize count;
        GCancellable *cancellable;
        GError *error = NULL;
        gsize bytes_read;

        SCAN_2_ARGS("02",
                    count, NUM2INT_DEFAULT_MINUS_1,
                    cancellable, RVAL2GCANCELLABLE);

        bytes_read = g_buffered_input_stream_fill(_SELF(self),
                                                  count,
                                                  cancellable,
                                                  &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return self;
}

DECLARE_VARG(fill_async)
{
        gssize count;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("03",
                               count, NUM2INT_DEFAULT_MINUS_1,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_buffered_input_stream_fill_async(_SELF(self),
                                           count,
                                           io_priority,
                                           cancellable,
                                           rbgio_async_ready_callback,
                                           (gpointer)block);
        return self;
}

DECLARE(fill_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        gsize bytes_read;

        bytes_read = g_buffered_input_stream_fill_finish(_SELF(self),
                                                         RVAL2GASYNCRESULT(result),
                                                         &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return UINT2NUM(bytes_read);
}

DECLARE_VARG(read_byte)
{
        GCancellable *cancellable;
        GError *error = NULL;
        int byte;

        SCAN_1_ARG("01",
                   cancellable, RVAL2GCANCELLABLE);

        byte = g_buffered_input_stream_read_byte(_SELF(self),
                                                 cancellable,
                                                 &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return INT2FIX(byte);
}

void
Init_gbufferedinputstream(VALUE glib)
{
        VALUE bufferedinputstream = G_DEF_CLASS(G_TYPE_BUFFERED_INPUT_STREAM,
                                                "BufferedInputStream",
                                                glib);

        DEF_METHOD(bufferedinputstream, initialize, -1);
        DEF_ATTRIBUTE(bufferedinputstream, buffer_size);
        DEF_METHOD(bufferedinputstream, available, 0);
        DEF_METHOD(bufferedinputstream, peek_buffer, 0);
        DEF_METHOD(bufferedinputstream, peek, 2);
        DEF_METHOD(bufferedinputstream, fill, -1);
        DEF_METHOD(bufferedinputstream, fill_async, -1);
        DEF_METHOD(bufferedinputstream, fill_finish, 1);
        DEF_METHOD(bufferedinputstream, read_byte, -1);
}
