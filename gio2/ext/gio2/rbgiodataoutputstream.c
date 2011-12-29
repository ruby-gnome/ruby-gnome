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

#define RG_TARGET_NAMESPACE cDataOutputStream
#define _SELF(value) RVAL2GDATAOUTPUTSTREAM(value)

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
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
rg_put_byte(int argc, VALUE *argv, VALUE self)
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
rg_put_int16(int argc, VALUE *argv, VALUE self)
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
rg_put_uint16(int argc, VALUE *argv, VALUE self)
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
rg_put_int32(int argc, VALUE *argv, VALUE self)
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
rg_put_uint32(int argc, VALUE *argv, VALUE self)
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
rg_put_int64(int argc, VALUE *argv, VALUE self)
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
rg_put_uint64(int argc, VALUE *argv, VALUE self)
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
rg_put_string(int argc, VALUE *argv, VALUE self)
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
Init_gdataoutputstream(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_DATA_OUTPUT_STREAM, "DataOutputStream", mGio);

        RG_DEF_METHOD(initialize, -1);
        RG_DEF_METHOD(put_byte, -1);
        RG_DEF_METHOD(put_int16, -1);
        RG_DEF_METHOD(put_uint16, -1);
        RG_DEF_METHOD(put_int32, -1);
        RG_DEF_METHOD(put_uint32, -1);
        RG_DEF_METHOD(put_int64, -1);
        RG_DEF_METHOD(put_uint64, -1);
        RG_DEF_METHOD(put_string, -1);
}
