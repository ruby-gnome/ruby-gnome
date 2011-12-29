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

#define RG_TARGET_NAMESPACE cBufferedInputStream
#define _SELF(value) RVAL2GBUFFEREDINPUTSTREAM(value)

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE rbbase_stream, size;
        GInputStream *base_stream, *stream;

        rb_scan_args(argc, argv, "11", &rbbase_stream, &size);
        base_stream = RVAL2GINPUTSTREAM(rbbase_stream);

        stream = NIL_P(size) ?
                g_buffered_input_stream_new(base_stream) :
                g_buffered_input_stream_new_sized(base_stream, RVAL2GSIZE(size));
        G_INITIALIZE(self, stream);

        return Qnil;
}

static VALUE
rg_available(VALUE self)
{
        return GSIZE2RVAL(g_buffered_input_stream_get_available(_SELF(self)));
}

static VALUE
rg_peek_buffer(VALUE self)
{
        gsize size;
        const void *buffer;

        buffer = g_buffered_input_stream_peek_buffer(_SELF(self), &size);

        return CSTR2RVAL_TAINTED(buffer, size);
}

static VALUE
rg_peek(VALUE self, VALUE rboffset, VALUE rbcount)
{
        gsize offset = RVAL2GSIZE(rboffset);
        gsize count = RVAL2GSIZE(rbcount);
        VALUE result = rb_str_new(NULL, count);
        gsize bytes_peeked = g_buffered_input_stream_peek(_SELF(self),
                                                          RSTRING_PTR(result),
                                                          offset,
                                                          count);

        rb_str_set_len(result, bytes_peeked);
        rb_str_resize(result, bytes_peeked);
        OBJ_TAINT(result);

        return result;
}

static VALUE
rg_fill(int argc, VALUE *argv, VALUE self)
{
        VALUE count, cancellable;
        GError *error = NULL;
        gsize bytes_read;

        rb_scan_args(argc, argv, "02", &count, &cancellable);
        bytes_read = g_buffered_input_stream_fill(_SELF(self),
                                                  NIL_P(count) ?
                                                  -1 :
                                                  RVAL2GSSIZE(count),
                                                  RVAL2GCANCELLABLE(cancellable),
                                                  &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GSSIZE2RVAL(bytes_read);
}

static VALUE
rg_fill_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcount, rbio_priority, rbcancellable, block;
        gssize count;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03&", &rbcount, &rbio_priority, &rbcancellable, &block);
        count = NIL_P(rbcount) ? -1 : RVAL2GSSIZE(rbcount);
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
rg_fill_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        gsize bytes_read;

        bytes_read = g_buffered_input_stream_fill_finish(_SELF(self),
                                                         RVAL2GASYNCRESULT(result),
                                                         &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GSSIZE2RVAL(bytes_read);
}

static VALUE
rg_read_byte(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        int byte;

        rb_scan_args(argc, argv, "01", &cancellable);
        byte = g_buffered_input_stream_read_byte(_SELF(self),
                                                 RVAL2GCANCELLABLE(cancellable),
                                                 &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return INT2FIX(byte);
}

void
Init_gbufferedinputstream(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_BUFFERED_INPUT_STREAM, "BufferedInputStream", mGio);

        RG_DEF_METHOD(initialize, -1);
        RG_DEF_METHOD(available, 0);
        RG_DEF_METHOD(peek_buffer, 0);
        RG_DEF_METHOD(peek, 2);
        RG_DEF_METHOD(fill, -1);
        RG_DEF_METHOD(fill_async, -1);
        RG_DEF_METHOD(fill_finish, 1);
        RG_DEF_METHOD(read_byte, -1);
}
