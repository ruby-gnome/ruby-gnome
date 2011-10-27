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

#include "gio2.h"

#define RG_TARGET_NAMESPACE cDataInputStream
#define _SELF(value) G_DATA_INPUT_STREAM(RVAL2GOBJ(value))

#define RVAL2GDATASTREAMNEWLINETYPE(value) \
        GENUM2RVAL((value), G_TYPE_DATA_STREAM_NEWLINE_TYPE)

static VALUE
datainputstream_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE base_stream, byte_order, newline_type;

        rb_scan_args(argc, argv, "12", &base_stream, &byte_order, &newline_type);

        G_INITIALIZE(self, g_data_input_stream_new(RVAL2GINPUTSTREAM(base_stream)));

        if (!NIL_P(byte_order))
                g_data_input_stream_set_byte_order(_SELF(self),
                                                   RVAL2GDATASTREAMBYTEORDER(byte_order));

        if (!NIL_P(newline_type))
                g_data_input_stream_set_newline_type(_SELF(self),
                                                     RVAL2GDATASTREAMNEWLINETYPE(newline_type));

        return Qnil;
}

static VALUE
datainputstream_read_byte(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        guchar value;

        rb_scan_args(argc, argv, "01", &cancellable);
        value = g_data_input_stream_read_byte(_SELF(self),
                                              RVAL2GCANCELLABLE(cancellable),
                                              &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GUCHAR2RVAL(value);
}

static VALUE
datainputstream_read_int16(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        gint16 value;

        rb_scan_args(argc, argv, "01", &cancellable);
        value = g_data_input_stream_read_int16(_SELF(self),
                                               RVAL2GCANCELLABLE(cancellable),
                                               &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GINT162RVAL(value);
}

static VALUE
datainputstream_read_uint16(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        guint16 value;

        rb_scan_args(argc, argv, "01", &cancellable);
        value = g_data_input_stream_read_uint16(_SELF(self),
                                                RVAL2GCANCELLABLE(cancellable),
                                                &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GUINT162RVAL(value);
}

static VALUE
datainputstream_read_int32(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        gint32 value;

        rb_scan_args(argc, argv, "01", &cancellable);
        value = g_data_input_stream_read_int32(_SELF(self),
                                               RVAL2GCANCELLABLE(cancellable),
                                               &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GINT322RVAL(value);
}

static VALUE
datainputstream_read_uint32(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        guint32 value;

        rb_scan_args(argc, argv, "01", &cancellable);
        value = g_data_input_stream_read_uint32(_SELF(self),
                                                RVAL2GCANCELLABLE(cancellable),
                                                &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GUINT322RVAL(value);
}

static VALUE
datainputstream_read_int64(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        gint64 value;

        rb_scan_args(argc, argv, "01", &cancellable);
        value = g_data_input_stream_read_int64(_SELF(self),
                                               RVAL2GCANCELLABLE(cancellable),
                                               &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GINT642RVAL(value);
}

static VALUE
datainputstream_read_uint64(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        guint64 value;

        rb_scan_args(argc, argv, "01", &cancellable);
        value = g_data_input_stream_read_uint64(_SELF(self),
                                                RVAL2GCANCELLABLE(cancellable),
                                                &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GUINT642RVAL(value);
}

static VALUE
datainputstream_read_line(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        gsize length;
        GError *error;
        char *line;

        rb_scan_args(argc, argv, "01", &cancellable);
        line = g_data_input_stream_read_line(_SELF(self),
                                             &length,
                                             RVAL2GCANCELLABLE(cancellable),
                                             &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return CSTR2RVAL_TAINTED_FREE(line, length);
}

static VALUE
datainputstream_read_line_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "02&", &rbio_priority, &rbcancellable, &block);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_data_input_stream_read_line_async(_SELF(self),
                                            io_priority,
                                            cancellable,
                                            rbgio_async_ready_callback,
                                            (gpointer)block);

        return self;
}

static VALUE
datainputstream_read_line_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        gsize length;
        char *line;

        line = g_data_input_stream_read_line_finish(_SELF(self),
                                                    RVAL2GASYNCRESULT(result),
                                                    &length,
                                                    &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return CSTR2RVAL_TAINTED_FREE(line, length);
}

#if GLIB_CHECK_VERSION(2, 26, 0)
static VALUE
datainputstream_read_upto(int argc, VALUE *argv, VALUE self)
{
        VALUE rbstop_chars, cancellable;
        const char *stop_chars;
        gsize length;
        GError *error;
        char *string;

        rb_scan_args(argc, argv, "11", &rbstop_chars, &cancellable);
        stop_chars = RVAL2CSTR(rbstop_chars);
        string = g_data_input_stream_read_upto(_SELF(self),
                                               stop_chars,
                                               RSTRING_LEN(rbstop_chars),
                                               &length,
                                               RVAL2GCANCELLABLE(cancellable),
                                               &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return CSTR2RVAL_TAINTED_FREE(string, length);
}

static VALUE
datainputstream_read_upto_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbstop_chars, rbcancellable, rbio_priority, block;
        const char *stop_chars;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "12&", &rbstop_chars, &rbio_priority, &rbcancellable, &block);
        stop_chars = RVAL2CSTR(rbstop_chars);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_data_input_stream_read_upto_async(_SELF(self),
                                            stop_chars,
                                            RSTRING_LEN(rbstop_chars),
                                            io_priority,
                                            cancellable,
                                            rbgio_async_ready_callback,
                                            (gpointer)block);

        return self;
}

static VALUE
datainputstream_read_upto_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        gsize length;
        char *string;

        string = g_data_input_stream_read_upto_finish(_SELF(self),
                                                      RVAL2GASYNCRESULT(result),
                                                      &length,
                                                      &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return CSTR2RVAL_TAINTED_FREE(string, length);
}
#endif

void
Init_gdatainputstream(VALUE glib)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_DATA_INPUT_STREAM, "DataInputStream", glib);

        rb_define_method(RG_TARGET_NAMESPACE, "initialize", datainputstream_initialize, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_byte", datainputstream_read_byte, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_int16", datainputstream_read_int16, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_uint16", datainputstream_read_uint16, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_int32", datainputstream_read_int32, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_uint32", datainputstream_read_uint32, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_int64", datainputstream_read_int64, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_uint64", datainputstream_read_uint64, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_line", datainputstream_read_line, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_line_async", datainputstream_read_line_async, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_line_finish", datainputstream_read_line_finish, 1);
#if GLIB_CHECK_VERSION(2, 26, 0)
        rb_define_method(RG_TARGET_NAMESPACE, "read_upto", datainputstream_read_upto, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_upto_async", datainputstream_read_upto_async, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "read_upto_finish", datainputstream_read_upto_finish, 1);
#endif
}
