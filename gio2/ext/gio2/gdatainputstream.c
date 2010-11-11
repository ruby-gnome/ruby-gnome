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

#define _SELF(value) G_DATA_INPUT_STREAM(RVAL2GOBJ(value))

#define GDATASTREAMNEWLINETYPE2RVAL(value) \
        RVAL2GENUM((value), G_TYPE_DATA_STREAM_NEWLINE_TYPE)

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
datainputstream_get_byte_order(VALUE self)
{
        return GDATASTREAMBYTEORDER2RVAL(g_data_input_stream_get_byte_order(_SELF(self)));
}

static VALUE
datainputstream_set_byte_order(VALUE self, VALUE value)
{
        g_data_input_stream_set_byte_order(_SELF(self),
                                           RVAL2GDATASTREAMBYTEORDER(value));

        return self;
}

static VALUE
datainputstream_set_newline_type(VALUE self, VALUE value)
{
        g_data_input_stream_set_newline_type(_SELF(self),
                                             RVAL2GDATASTREAMNEWLINETYPE(value));

        return self;
}

static VALUE
datainputstream_get_newline_type(VALUE self)
{
        return GDATASTREAMNEWLINETYPE2RVAL(g_data_input_stream_get_newline_type(_SELF(self)));
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

static VALUE
datainputstream_read_upto(int argc, VALUE *argv, VALUE self)
{
        VALUE stop_chars, cancellable;
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

void
Init_gdatainputstream(VALUE glib)
{
        VALUE datainputstream = G_DEF_CLASS(G_TYPE_DATA_INPUT_STREAM, "DataInputStream", glib);

        rb_define_method(datainputstream, "initialize", datainputstream_initialize, -1);
        rb_define_method(datainputstream, "byte_order", datainputstream_get_byte_order, 0);
        rb_define_method(datainputstream, "set_byte_order", datainputstream_set_byte_order, 1);
        G_DEF_SETTER(datainputstream, "byte_order");
        rb_define_method(datainputstream, "set_newline_type", datainputstream_set_newline_type, 1);
        G_DEF_SETTER(datainputstream, "newline_type");
        rb_define_method(datainputstream, "newline_type", datainputstream_get_newline_type, 0);
        rb_define_method(datainputstream, "read_byte", datainputstream_read_byte, -1);
        rb_define_method(datainputstream, "read_int16", datainputstream_read_int16, -1);
        rb_define_method(datainputstream, "read_uint16", datainputstream_read_uint16, -1);
        rb_define_method(datainputstream, "read_int32", datainputstream_read_int32, -1);
        rb_define_method(datainputstream, "read_uint32", datainputstream_read_uint32, -1);
        rb_define_method(datainputstream, "read_int64", datainputstream_read_int64, -1);
        rb_define_method(datainputstream, "read_uint64", datainputstream_read_uint64, -1);
        rb_define_method(datainputstream, "read_line", datainputstream_read_line, -1);
        rb_define_method(datainputstream, "read_line_async", datainputstream_read_line_async, -1);
        rb_define_method(datainputstream, "read_line_finish", datainputstream_read_line_finish, 1);
        rb_define_method(datainputstream, "read_upto", datainputstream_read_upto, -1);
        rb_define_method(datainputstream, "read_upto_async", datainputstream_read_upto_async, -1);
        rb_define_method(datainputstream, "read_upto_finish", datainputstream_read_upto_finish, 1);
}
