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
stream_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE base_stream,
              byte_order;

        rb_scan_args(argc, argv, "11", &base_stream, &byte_order);

        G_INITIALIZE(self,
                     g_data_output_stream_new(RVAL2GOUTPUTSTREAM(base_stream)));

        if (!NIL_P(byte_order))
                g_data_output_stream_set_byte_order(_SELF(self),
                                                    RVAL2GDATASTREAMBYTEORDER(byte_order));

        return Qnil;
}

static VALUE
stream_byte_order(VALUE self)
{
	return GDATASTREAMBYTEORDER2RVAL(g_data_output_stream_get_byte_order(_SELF(self)));
}

static VALUE
stream_set_byte_order(VALUE self, VALUE value)
{
	g_data_output_stream_set_byte_order(_SELF(self), RVAL2GDATASTREAMBYTEORDER(value));

	return self;
}

static VALUE
stream_put_byte(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_byte(_SELF(self), FIX2UINT(value),
                                           RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
stream_put_int16(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_int16(_SELF(self), FIX2UINT(value),
                                            RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
stream_put_uint16(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_uint16(_SELF(self), FIX2UINT(value),
                                             RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
stream_put_int32(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_int16(_SELF(self), NUM2INT(value),
                                            RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
stream_put_uint32(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_int16(_SELF(self), NUM2UINT(value),
                                            RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
stream_put_int64(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_int64(_SELF(self), rbglib_num_to_int64(value),
                                            RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
stream_put_uint64(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_int16(_SELF(self), rbglib_num_to_uint64(value),
                                            RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
stream_put_string(int argc, VALUE *argv, VALUE self)
{
        VALUE value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);
        if (!g_data_output_stream_put_string(_SELF(self), RVAL2CSTR(value),
                                             RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_io_error(error);

        return self;
}

#if 0
static VALUE
put_value(int argc, VALUE *argv, VALUE self)
{
        VALUE value,
              cancellable;
        gboolean success;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &value, &cancellable);

        /* TODO: How do we convert from a Number to uint64 through byte in a
         * reliable manner?  Is it even sane to do so? */
        switch (TYPE(value)) {
        case T_STRING:
                success = g_data_output_stream_put_string(_SELF(self),
                                                          RVAL2CSTR(value),
                                                          cancellable,
                                                          &error);
                break;

        if (!success)
                rbgio_raise_io_error(error);

        return self;
}
#endif

void
Init_gdataoutputstream(VALUE glib)
{
        VALUE dataoutputstream = G_DEF_CLASS(G_TYPE_DATA_OUTPUT_STREAM, "DataOutputStream", glib);

        rb_define_method(dataoutputstream, "initialize", stream_initialize, -1);
        rb_define_method(dataoutputstream, "byte_order", stream_byte_order, 0);
	rb_define_method(dataoutputstream, "set_byte_order", stream_set_byte_order, 1);
	G_DEF_SETTER(dataoutputstream, "byte_order");
        rb_define_method(dataoutputstream, "put_byte", stream_put_byte, -1);
        rb_define_method(dataoutputstream, "put_int16", stream_put_int16, -1);
        rb_define_method(dataoutputstream, "put_uint16", stream_put_uint16, -1);
        rb_define_method(dataoutputstream, "put_int32", stream_put_int32, -1);
        rb_define_method(dataoutputstream, "put_uint32", stream_put_uint32, -1);
        rb_define_method(dataoutputstream, "put_int64", stream_put_int64, -1);
        rb_define_method(dataoutputstream, "put_uint64", stream_put_uint64, -1);
        rb_define_method(dataoutputstream, "put_string", stream_put_string, -1);
        rb_define_alias(dataoutputstream, "<<", "put_string");
        rb_define_alias(dataoutputstream, "write", "put_string");

        /* TODO: Should we add #put_value/#put? */
}
