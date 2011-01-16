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

#define _SELF(value) RVAL2GOUTPUTSTREAM(value)

#define RVAL2GOUTPUTSTREAMSPLICEFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_OUTPUT_STREAM_SPLICE_FLAGS)

#define RVAL2GOUTPUTSTREAMSPLICEFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GOUTPUTSTREAMSPLICEFLAGS, \
                               G_OUTPUT_STREAM_SPLICE_NONE)

static VALUE
outputstream_write(int argc, VALUE *argv, VALUE self)
{
        VALUE rbbuffer, cancellable;
        const char *buffer;
        GError *error = NULL;
        gssize bytes_written;

        rb_scan_args(argc, argv, "11", &rbbuffer, &cancellable);
        buffer = RVAL2CSTR(rbbuffer);
        bytes_written = g_output_stream_write(_SELF(self),
                                              buffer,
                                              (gsize)RSTRING_LEN(rbbuffer),
                                              RVAL2GCANCELLABLE(cancellable),
                                              &error);
        if (bytes_written == -1)
                rbgio_raise_error(error);

        return GSSIZE2RVAL(bytes_written);
}

static VALUE
outputstream_write_all(int argc, VALUE *argv, VALUE self)
{
        VALUE rbbuffer, cancellable;
        const char *buffer;
        gsize bytes_written;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &rbbuffer, &cancellable);
        buffer = RVAL2CSTR(rbbuffer);
        if (!g_output_stream_write_all(_SELF(self),
                                       buffer,
                                       (gsize)RSTRING_LEN(rbbuffer),
                                       &bytes_written,
                                       RVAL2GCANCELLABLE(cancellable),
                                       &error))
                rbgio_raise_error(error);

        return GSIZE2RVAL(bytes_written);
}

static VALUE
outputstream_splice(int argc, VALUE *argv, VALUE self)
{
        VALUE source, flags, cancellable;
        GError *error = NULL;
        gssize bytes_spliced;

        rb_scan_args(argc, argv, "12", &source, &flags, &cancellable);
        bytes_spliced = g_output_stream_splice(_SELF(self),
                                               RVAL2GINPUTSTREAM(source),
                                               RVAL2GOUTPUTSTREAMSPLICEFLAGSDEFAULT(flags),
                                               RVAL2GCANCELLABLE(cancellable),
                                               &error);
        if (bytes_spliced == -1)
                rbgio_raise_error(error);

        return GSSIZE2RVAL(bytes_spliced);
}

typedef gboolean (*CancellableMethod)(GOutputStream *, GCancellable *, GError **);

static VALUE
cancellable_method(CancellableMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "01", &cancellable);
        if (!method(_SELF(self), RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
outputstream_flush(int argc, VALUE *argv, VALUE self)
{
        return cancellable_method(g_output_stream_flush, argc, argv, self);
}

static VALUE
outputstream_close(int argc, VALUE *argv, VALUE self)
{
        return cancellable_method(g_output_stream_close, argc, argv, self);
}

/* TODO: Does it make sense to use buffer and count?  We should probably
 * provide a better wrapper that simply pumps out buffer while count hasn’t
 * been reached, calling the callback with the bytes written, then with the
 * result. */
static VALUE
outputstream_write_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbbuffer, rbcount, rbio_priority, rbcancellable, block;
        const gchar *buffer;
        gsize count;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "22&", &rbbuffer, &rbcount, &rbio_priority, &rbcancellable, &block);
        buffer = RVAL2CSTR(rbbuffer);
        count = RVAL2GSIZE(rbcount);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_output_stream_write_async(_SELF(self),
                                    buffer,
                                    count,
                                    io_priority,
                                    cancellable,
                                    rbgio_async_ready_callback,
                                    (gpointer)block);

        return self;
}

typedef gssize (*SSizeFinishMethod)(GOutputStream *, GAsyncResult *, GError **);

static VALUE
ssize_finish_method(SSizeFinishMethod method, VALUE self, VALUE result)
{
        GError *error = NULL;
        gssize bytes_read;

        bytes_read = method(_SELF(self), RVAL2GASYNCRESULT(result), &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GSSIZE2RVAL(bytes_read);
}

static VALUE
outputstream_write_finish(VALUE self, VALUE result)
{
        return ssize_finish_method(g_output_stream_write_finish, self, result);
}

static VALUE
outputstream_splice_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbsource, rbflags, rbio_priority, rbcancellable, block;
        GInputStream *source;
        GOutputStreamSpliceFlags flags;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "12&", &rbsource, &rbflags, &rbio_priority, &rbcancellable, &block);
        source = RVAL2GINPUTSTREAM(rbsource);
        flags = RVAL2GOUTPUTSTREAMSPLICEFLAGSDEFAULT(rbflags);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_output_stream_splice_async(_SELF(self),
                                     source,
                                     flags,
                                     io_priority,
                                     cancellable,
                                     rbgio_async_ready_callback,
                                     (gpointer)block);

        return self;
}

static VALUE
outputstream_splice_finish(VALUE self, VALUE result)
{
        return ssize_finish_method(g_output_stream_splice_finish, self, result);
}

static VALUE
outputstream_flush_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "02&", &rbio_priority, &rbcancellable, &block);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_output_stream_flush_async(_SELF(self),
                                    io_priority,
                                    cancellable,
                                    rbgio_async_ready_callback,
                                    (gpointer)block);

        return self;
}

typedef gboolean (*BooleanFinishMethod)(GOutputStream *, GAsyncResult *, GError **);

static VALUE
boolean_finish_method(BooleanFinishMethod method, VALUE self, VALUE result)
{
        GError *error = NULL;

        if (!method(_SELF(self), RVAL2GASYNCRESULT(result), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
outputstream_flush_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_output_stream_flush_finish, self, result);
}

static VALUE
outputstream_close_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "02&", &rbio_priority, &rbcancellable, &block);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_output_stream_close_async(_SELF(self),
                                    io_priority,
                                    cancellable,
                                    rbgio_async_ready_callback,
                                    (gpointer)block);

        return self;
}

static VALUE
outputstream_close_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_output_stream_close_finish, self, result);
}

static VALUE
outputstream_is_closing(VALUE self)
{
        return CBOOL2RVAL(g_output_stream_is_closing(_SELF(self)));
}

static VALUE
outputstream_is_closed(VALUE self)
{
        return CBOOL2RVAL(g_output_stream_is_closed(_SELF(self)));
}

static VALUE
outputstream_has_pending(VALUE self)
{
        return CBOOL2RVAL(g_output_stream_has_pending(_SELF(self)));
}

static VALUE
outputstream_set_pending(VALUE self)
{
        GError *error = NULL;

        if (!g_output_stream_set_pending(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
outputstream_clear_pending(VALUE self)
{
        g_output_stream_clear_pending(_SELF(self));

        return self;
}

void
Init_goutputstream(VALUE glib)
{
        VALUE outputstream = G_DEF_CLASS(G_TYPE_OUTPUT_STREAM, "OutputStream", glib);

        G_DEF_CLASS(G_TYPE_OUTPUT_STREAM_SPLICE_FLAGS, "SpliceFlags", outputstream);
        G_DEF_CONSTANTS(outputstream, G_TYPE_OUTPUT_STREAM_SPLICE_FLAGS, "G_OUTPUT_STREAM_");

        rb_define_method(outputstream, "write", outputstream_write, -1);
        rb_define_method(outputstream, "write_all", outputstream_write_all, -1);
        rb_define_method(outputstream, "splice", outputstream_splice, -1);
        rb_define_method(outputstream, "flush", outputstream_flush, -1);
        rb_define_method(outputstream, "close", outputstream_close, -1);
        rb_define_method(outputstream, "write_async", outputstream_write_async, -1);
        rb_define_method(outputstream, "write_finish", outputstream_write_finish, 1);
        rb_define_method(outputstream, "splice_async", outputstream_splice_async, -1);
        rb_define_method(outputstream, "splice_finish", outputstream_splice_finish, 1);
        rb_define_method(outputstream, "flush_async", outputstream_flush_async, -1);
        rb_define_method(outputstream, "flush_finish", outputstream_flush_finish, 1);
        rb_define_method(outputstream, "close_async", outputstream_close_async, -1);
        rb_define_method(outputstream, "close_finish", outputstream_close_finish, 0);
        rb_define_method(outputstream, "closing?", outputstream_is_closing, 0);
        rb_define_method(outputstream, "closed?", outputstream_is_closed, 0);
        rb_define_method(outputstream, "has_pending?", outputstream_has_pending, 0);
        rb_define_method(outputstream, "set_pending", outputstream_set_pending, 0);
        G_DEF_SETTER(outputstream, "pending");
        rb_define_method(outputstream, "clear_pending", outputstream_clear_pending, 0);
}
