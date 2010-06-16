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

#define _SELF(value) G_BUFFERED_INPUT_STREAM(RVAL2GOBJ(value))

static VALUE
stream_initialize(int argc, VALUE *argv, VALUE self)
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

static VALUE
stream_buffer_size(VALUE  self)
{
	return UINT2NUM(g_buffered_input_stream_get_buffer_size(_SELF(self)));
}


static VALUE
stream_set_buffer_size(VALUE self, VALUE size)
{
        g_buffered_input_stream_set_buffer_size(_SELF(self), NUM2UINT(size));

        return self;
}

static VALUE
stream_available(VALUE  self)
{
	return UINT2NUM(g_buffered_input_stream_get_available(_SELF(self)));
}


static VALUE
stream_peek_buffer(VALUE self)
{
        gsize size;
        const void *buffer;

        buffer = g_buffered_input_stream_peek_buffer(_SELF(self), &size);

        return CSTR2RVAL_OWN_FROZEN((const char *)buffer, size);
}

static VALUE
stream_peek(VALUE self, VALUE rboffset, VALUE rbcount)
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

static VALUE
stream_fill(int argc, VALUE *argv, VALUE self)
{
        VALUE count, cancellable;
        GError *error = NULL;
        gsize bytes_read;

        rb_scan_args(argc, argv, "02", &count, &cancellable);
        bytes_read = g_buffered_input_stream_fill(_SELF(self),
                                                  NUM2INT_DEFAULT_MINUS_1(count),
                                                  RVAL2GCANCELLABLE(cancellable),
                                                  &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return UINT2NUM(bytes_read);
}

static VALUE
stream_fill_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcount, rbio_priority, rbcancellable, block;
        gssize count;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03&", &rbcount, &rbio_priority, &rbcancellable, &block);
        count = NUM2INT_DEFAULT_MINUS_1(rbcount);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);

        g_buffered_input_stream_fill_async(_SELF(self),
                                           count,
                                           io_priority,
                                           cancellable,
                                           rbgio_async_ready_callback,
                                           (gpointer)block);

        return self;
}

static VALUE
stream_fill_finish(VALUE self, VALUE result)
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

static VALUE
stream_read_byte(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        int byte;

        rb_scan_args(argc, argv, "01", &cancellable);
        byte = g_buffered_input_stream_read_byte(_SELF(self),
                                                 RVAL2GCANCELLABLE(cancellable),
                                                 &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return INT2FIX(byte);
}

void
Init_gbufferedinputstream(VALUE glib)
{
        VALUE bufferedinputstream = G_DEF_CLASS(G_TYPE_BUFFERED_INPUT_STREAM, "BufferedInputStream", glib);

        rb_define_method(bufferedinputstream, "initialize", stream_initialize, -1);
        rb_define_method(bufferedinputstream, "buffer_size", stream_buffer_size, 0);
	rb_define_method(bufferedinputstream, "set_buffer_size", stream_set_buffer_size, 1);
	G_DEF_SETTER(bufferedinputstream, "buffer_size");
        rb_define_method(bufferedinputstream, "available", stream_available, 0);
        rb_define_method(bufferedinputstream, "peek_buffer", stream_peek_buffer, 0);
        rb_define_method(bufferedinputstream, "peek", stream_peek, 2);
        rb_define_method(bufferedinputstream, "fill", stream_fill, -1);
        rb_define_method(bufferedinputstream, "fill_async", stream_fill_async, -1);
        rb_define_method(bufferedinputstream, "fill_finish", stream_fill_finish, 1);
        rb_define_method(bufferedinputstream, "read_byte", stream_read_byte, -1);
}
