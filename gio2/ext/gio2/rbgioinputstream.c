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

#define RG_TARGET_NAMESPACE cInputStream
#define _SELF(value) RVAL2GINPUTSTREAM(value)

static VALUE s_cReadAsyncResult;

static VALUE
rg_read(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcount, cancellable, result;
        gsize count;
        GError *error = NULL;
        gssize bytes_read;

        rb_scan_args(argc, argv, "11", &rbcount, &cancellable);
        count = RVAL2GSIZE(rbcount);
        result = rb_str_new(NULL, count);
        bytes_read = g_input_stream_read(_SELF(self),
                                         RSTRING_PTR(result),
                                         count,
                                         RVAL2GCANCELLABLE(cancellable),
                                         &error);
        if (bytes_read == -1)
                rbgio_raise_error(error);

        rb_str_set_len(result, bytes_read);
        rb_str_resize(result, bytes_read);
        OBJ_TAINT(result);

        return result;
}

static VALUE
rg_read_all(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcount, cancellable, result;
        gsize count;
        GError *error = NULL;
        gsize bytes_read;

        rb_scan_args(argc, argv, "11", &rbcount, &cancellable);
        count = RVAL2GSIZE(rbcount);
        result = rb_str_new(NULL, count);
        if (!g_input_stream_read_all(_SELF(self),
                                     RSTRING_PTR(result),
                                     count,
                                     &bytes_read,
                                     RVAL2GCANCELLABLE(cancellable),
                                     &error))
                rbgio_raise_error(error);

        rb_str_set_len(result, bytes_read);
        rb_str_resize(result, bytes_read);
        OBJ_TAINT(result);

        return result;
}

static VALUE
rg_skip(int argc, VALUE *argv, VALUE self)
{
        VALUE count, cancellable;
        GError *error = NULL;
        gssize bytes_skipped;

        rb_scan_args(argc, argv, "11", &count, &cancellable);
        bytes_skipped = g_input_stream_skip(_SELF(self),
                                            RVAL2GSIZE(count),
                                            RVAL2GCANCELLABLE(cancellable),
                                            &error);
        if (bytes_skipped == -1)
                rbgio_raise_error(error);

        return GSSIZE2RVAL(bytes_skipped);
}

static VALUE
rg_close(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "01", &cancellable);
        if (!g_input_stream_close(_SELF(self),
                                  RVAL2GCANCELLABLE(cancellable),
                                  &error))
                rbgio_raise_error(error);

        return self;
}

struct read_async_result
{
        VALUE string;
        GAsyncResult *result;
};

static void
read_async_result_mark(struct read_async_result *result)
{
        rb_gc_mark(result->string);
}

struct read_async_callback_data
{
        GAsyncResult *result;
        gpointer user_data;
};

static VALUE
read_async_callback_call(VALUE data)
{
        static ID s_id_call;
        struct read_async_callback_data *real;
        VALUE ary;
        struct read_async_result *result;

        if (s_id_call == 0)
                s_id_call = rb_intern("call");

        real = (struct read_async_callback_data *)data;

        ary = (VALUE)real->user_data;
        G_CHILD_REMOVE(mGLib, ary);
        if (NIL_P(RARRAY_PTR(ary)[1]))
                return Qnil;

        result = g_new(struct read_async_result, 1);
        result->string = RARRAY_PTR(ary)[0];
        result->result = real->result;

        rb_funcall(RARRAY_PTR(ary)[1], s_id_call, 1,
                   Data_Wrap_Struct(s_cReadAsyncResult,
                                    read_async_result_mark,
                                    g_free,
                                    result));

        return Qnil;
}

static void
read_async_callback(G_GNUC_UNUSED GObject *source,
                    GAsyncResult *result,
                    gpointer user_data)
{
        struct read_async_callback_data real = { result, user_data };

        G_PROTECT_CALLBACK(read_async_callback_call, &real);
}

static VALUE
rg_read_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcount, rbio_priority, rbcancellable, block, data;
        gsize count;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "12&", &rbcount, &rbio_priority, &rbcancellable, &block);
        count = RVAL2GSIZE(rbcount);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        data = rb_assoc_new(rb_str_new(NULL, count), block);
        G_CHILD_ADD(mGLib, data);
        g_input_stream_read_async(_SELF(self),
                                  RSTRING_PTR(RARRAY_PTR(data)[0]),
                                  count,
                                  io_priority,
                                  cancellable,
                                  read_async_callback,
                                  (gpointer)data);

        return self;
}

static VALUE
rg_read_finish(VALUE self, VALUE rbresult)
{
        struct read_async_result *result;
        GError *error = NULL;
        gssize bytes_read;

        Data_Get_Struct(rbresult, struct read_async_result, result);
        bytes_read = g_input_stream_read_finish(_SELF(self),
                                                result->result,
                                                &error);
        if (bytes_read == -1)
                rbgio_raise_error(error);

        rb_str_set_len(result->string, bytes_read);
        rb_str_resize(result->string, bytes_read);
        OBJ_TAINT(result->string);

        return result->string;
}

static VALUE
rg_skip_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcount, rbio_priority, rbcancellable, block;
        gsize count;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "12&", &rbcount, &rbio_priority, &rbcancellable, &block);
        count = RVAL2GSIZE(rbcount);
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
rg_skip_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        gssize skipped_bytes;

        skipped_bytes = g_input_stream_skip_finish(_SELF(self),
                                                   RVAL2GASYNCRESULT(result),
                                                   &error);
        if (skipped_bytes == -1)
                rbgio_raise_error(error);

        return GSSIZE2RVAL(skipped_bytes);
}

static VALUE
rg_close_async(int argc, VALUE *argv, VALUE self)
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
rg_close_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_input_stream_skip_finish(_SELF(self),
                                        RVAL2GASYNCRESULT(result),
                                        &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_closed_p(VALUE self)
{
        return CBOOL2RVAL(g_input_stream_is_closed(_SELF(self)));
}

static VALUE
rg_has_pending_p(VALUE self)
{
        return CBOOL2RVAL(g_input_stream_has_pending(_SELF(self)));
}

static VALUE
rg_set_pending(VALUE self)
{
        GError *error = NULL;

        if (!g_input_stream_set_pending(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_clear_pending(VALUE self)
{
        g_input_stream_clear_pending(_SELF(self));

        return self;
}

void
Init_ginputstream(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_INPUT_STREAM, "InputStream", mGio);

        s_cReadAsyncResult = rb_define_class_under(RG_TARGET_NAMESPACE, "ReadAsyncResult", rb_cObject);

        RG_DEF_METHOD(read, -1);
        RG_DEF_METHOD(read_all, -1);
        RG_DEF_METHOD(skip, -1);
        RG_DEF_METHOD(close, -1);
        RG_DEF_METHOD(read_async, -1);
        RG_DEF_METHOD(read_finish, 1);
        RG_DEF_METHOD(skip_async, -1);
        RG_DEF_METHOD(skip_finish, 1);
        RG_DEF_METHOD(close_async, -1);
        RG_DEF_METHOD(close_finish, 1);
        RG_DEF_METHOD_P(closed, 0);
        RG_DEF_METHOD_P(has_pending, 0);
        RG_DEF_METHOD(set_pending, 0);
        RG_DEF_METHOD(clear_pending, 0);
}
