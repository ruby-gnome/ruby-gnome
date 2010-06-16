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

#define _SELF(value) RVAL2GINPUTSTREAM(value)

static VALUE s_id_call;

static VALUE
stream_read(int argc, VALUE *argv, VALUE self)
{
        VALUE count, cancellable, buffer;
        GError *error = NULL;
        gssize bytes_read;

        rb_scan_args(argc, argv, "11", &count, &cancellable);

        /* TODO: Correct? */
        buffer = rb_str_buf_new(NUM2UINT(count));

        bytes_read = g_input_stream_read(_SELF(self),
                                         RSTRING_PTR(buffer),
                                         NUM2UINT(count),
                                         RVAL2GCANCELLABLE(cancellable),
                                         &error);
        if (bytes_read == -1)
                rbgio_raise_io_error(error);

        return buffer;
}

static VALUE
stream_read_all(int argc, VALUE *argv, VALUE self)
{
        VALUE count, cancellable, buffer;
        GError *error = NULL;
        gsize bytes_read;

        rb_scan_args(argc, argv, "11", &count, &cancellable);

        buffer = rb_str_buf_new(NUM2UINT(count));

        if (!g_input_stream_read_all(_SELF(self),
                                     RSTRING_PTR(buffer),
                                     NUM2UINT(count),
                                     &bytes_read,
                                     RVAL2GCANCELLABLE(cancellable),
                                     &error))
                /* TODO: Should we include information about how many bytes
                 * were read (bytes_read)? */
                /* TODO: Should we free buffer? */
                rbgio_raise_io_error(error);

        return buffer;
}

static VALUE
stream_skip(int argc, VALUE *argv, VALUE self)
{
        VALUE count, cancellable;
        GError *error = NULL;
        gssize bytes_skipped;

        rb_scan_args(argc, argv, "11", &count, &cancellable);
        bytes_skipped = g_input_stream_skip(_SELF(self),
                                            NUM2UINT(count),
                                            RVAL2GCANCELLABLE(cancellable),
                                            &error);
        if (bytes_skipped == -1)
                rbgio_raise_io_error(error);

        return INT2NUM(bytes_skipped);
}

static VALUE
stream_close(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "01", &cancellable);
        if (!g_input_stream_close(_SELF(self), RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_io_error(error);

        return self;
}

static void
read_async_callback(UNUSED(GObject *source),
                    GAsyncResult *result,
                    gpointer user_data)
{
        VALUE data = (VALUE)user_data;

        if (NIL_P(data))
                return;

        G_CHILD_REMOVE(mGLib, data);

        /* TODO: I guess we should probably implement our own result object
         * here that gives you data[0] on #finish. */
        rb_funcall(RARRAY_PTR(data)[1], s_id_call, 2,
                   GOBJ2RVAL(result), RARRAY_PTR(data)[0]);
}

static VALUE
stream_read_async(int argc, VALUE *argv, VALUE self)
{
        VALUE count, io_priority, cancellable, data;

        rb_scan_args(argc, argv, "12", &count, &io_priority, &cancellable);

        if (rb_block_given_p()) {
                data = rb_assoc_new(rb_str_buf_new(NUM2UINT(count)), rb_block_proc());
                G_CHILD_ADD(mGLib, data);
        } else {
                data = Qnil;
        }

        /* TODO: This array/string pointer access can not be correct. */
        g_input_stream_read_async(_SELF(self),
                                  RSTRING_PTR(RARRAY_PTR(data)[0]),
                                  NUM2UINT(count),
                                  RVAL2IOPRIORITYDEFAULT(io_priority),
                                  RVAL2GCANCELLABLE(cancellable),
                                  read_async_callback,
                                  (gpointer)data);

        return self;
}

static VALUE
stream_read_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        gssize read_bytes;

        read_bytes = g_input_stream_read_finish(_SELF(self),
                                                RVAL2GASYNCRESULT(result),
                                                &error);
        if (read_bytes == -1)
                rbgio_raise_io_error(error);

        /* TODO: Return self instead? */
        return INT2NUM(read_bytes);
}

static VALUE
stream_skip_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcount, rbio_priority, rbcancellable, block;
        gsize count;
        int io_priority;
        GCancellable *cancellable;

	rb_scan_args(argc, argv, "12&", &rbcount, &rbio_priority, &rbcancellable, &block);
	count = NUM2UINT(rbcount);
	io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
	cancellable = RVAL2GCANCELLABLE(rbcancellable);
	SAVE_BLOCK(block);

        g_input_stream_skip_async(_SELF(self),
                                  count,
                                  io_priority,
                                  cancellable,
                                  rbgio_async_ready_callback,
                                  (gpointer)block);

        return self;
}

static VALUE
stream_skip_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        gssize skipped_bytes;

        skipped_bytes = g_input_stream_skip_finish(_SELF(self),
                                                   RVAL2GASYNCRESULT(result),
                                                   &error);
        if (skipped_bytes == -1)
                rbgio_raise_io_error(error);

        return INT2NUM(skipped_bytes);
}

static VALUE
stream_close_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        int io_priority;
        GCancellable *cancellable;

	rb_scan_args(argc, argv, "02&", &rbio_priority, &rbcancellable, &block);
	io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
	cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);

        g_input_stream_close_async(_SELF(self),
                                   io_priority,
                                   cancellable,
                                   rbgio_async_ready_callback,
                                   (gpointer)block);

        return self;
}

static VALUE
stream_close_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_input_stream_skip_finish(_SELF(self),
                                        RVAL2GASYNCRESULT(result),
                                        &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
stream_is_closed(VALUE self)
{
	return CBOOL2RVAL(g_input_stream_is_closed(_SELF(self)));
}

static VALUE
stream_has_pending(VALUE self)
{
	return CBOOL2RVAL(g_input_stream_has_pending(_SELF(self)));
}

static VALUE
stream_set_pending(VALUE self)
{
        GError *error = NULL;

        if (!g_input_stream_set_pending(_SELF(self), &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
stream_clear_pending(VALUE self)
{
        g_input_stream_clear_pending(_SELF(self));

        return self;
}

void
Init_ginputstream(VALUE glib)
{
        VALUE inputstream = G_DEF_CLASS(G_TYPE_INPUT_STREAM, "InputStream", glib);

        s_id_call = rb_intern("call");

        /* TODO: Undef alloct func? */

        rb_define_method(inputstream, "read", stream_read, -1);
        rb_define_method(inputstream, "read_all", stream_read_all, -1);
        rb_define_method(inputstream, "skip", stream_skip, -1);
        rb_define_method(inputstream, "close", stream_close, -1);
        rb_define_method(inputstream, "read_async", stream_read_async, -1);
        rb_define_method(inputstream, "read_finish", stream_read_finish, 1);
        rb_define_method(inputstream, "skip_async", stream_skip_async, -1);
        rb_define_method(inputstream, "skip_finish", stream_skip_finish, 1);
        rb_define_method(inputstream, "close_async", stream_close_async, -1);
        rb_define_method(inputstream, "close_finish", stream_close_finish, 1);
        rb_define_method(inputstream, "closed?", stream_is_closed, 0);
        rb_define_method(inputstream, "has_pending?", stream_has_pending, 0);
        rb_define_method(inputstream, "set_pending", stream_set_pending, 0);
        rb_define_method(inputstream, "clear_pending", stream_clear_pending, 0);
}
