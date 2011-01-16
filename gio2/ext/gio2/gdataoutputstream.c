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

#define _SELF(value) G_DATA_OUTPUT_STREAM(RVAL2GOBJ(value))

static VALUE
dataoutputstream_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE base_stream,
              byte_order;

        rb_scan_args(argc, argv, "11", &base_stream, &byte_order);

        G_INITIALIZE(self, g_data_output_stream_new(RVAL2GOUTPUTSTREAM(base_stream)));

        if (!NIL_P(byte_order))
                g_data_output_stream_set_byte_order(_SELF(self),
                                                    RVAL2GDATASTREAMBYTEORDER(byte_order));

        return Qnil;
}

static VALUE
dataoutputstream_put_byte(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_byte(_SELF(self),
                                           RVAL2GUCHAR(value),
                                           RVAL2GCANCELLABLE(cancellable),
                                           &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
dataoutputstream_put_int16(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_int16(_SELF(self),
                                            RVAL2GINT16(value),
                                            RVAL2GCANCELLABLE(cancellable),
                                            &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
dataoutputstream_put_uint16(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_uint16(_SELF(self),
                                             RVAL2GUINT16(value),
                                             RVAL2GCANCELLABLE(cancellable),
                                             &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
dataoutputstream_put_int32(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_int32(_SELF(self),
                                            RVAL2GINT32(value),
                                            RVAL2GCANCELLABLE(cancellable),
                                            &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
dataoutputstream_put_uint32(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_uint32(_SELF(self),
                                             RVAL2GINT32(value),
                                             RVAL2GCANCELLABLE(cancellable),
                                             &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
dataoutputstream_put_int64(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_int64(_SELF(self),
                                            RVAL2GINT64(value),
                                            RVAL2GCANCELLABLE(cancellable),
                                            &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
dataoutputstream_put_uint64(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_uint64(_SELF(self),
                                             RVAL2GUINT64(value),
                                             RVAL2GCANCELLABLE(cancellable),
                                             &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
dataoutputstream_put_string(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_string(_SELF(self),
                                             RVAL2CSTR(value),
                                             RVAL2GCANCELLABLE(cancellable),
                                             &error))
                rbgio_raise_error(error);

        return self;
}

void
Init_gdataoutputstream(VALUE glib)
{
        VALUE dataoutputstream = G_DEF_CLASS(G_TYPE_DATA_OUTPUT_STREAM, "DataOutputStream", glib);

        rb_define_method(dataoutputstream, "initialize", dataoutputstream_initialize, -1);
        rb_define_method(dataoutputstream, "put_byte", dataoutputstream_put_byte, -1);
        rb_define_method(dataoutputstream, "put_int16", dataoutputstream_put_int16, -1);
        rb_define_method(dataoutputstream, "put_uint16", dataoutputstream_put_uint16, -1);
        rb_define_method(dataoutputstream, "put_int32", dataoutputstream_put_int32, -1);
        rb_define_method(dataoutputstream, "put_uint32", dataoutputstream_put_uint32, -1);
        rb_define_method(dataoutputstream, "put_int64", dataoutputstream_put_int64, -1);
        rb_define_method(dataoutputstream, "put_uint64", dataoutputstream_put_uint64, -1);
        rb_define_method(dataoutputstream, "put_string", dataoutputstream_put_string, -1);
}
