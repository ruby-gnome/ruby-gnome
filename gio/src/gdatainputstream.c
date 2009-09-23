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

#define _SELF(value) \
        G_DATA_INPUT_STREAM(RVAL2GOBJ(value))

DECLARE_VARG(initialize)
{
        VALUE base_stream,
              byte_order,
              newline_type;

        rb_scan_args(argc, argv, "12",
                     &base_stream, &byte_order, &newline_type);

        G_INITIALIZE(self,
                     g_data_input_stream_new(RVAL2GINPUTSTREAM(base_stream)));

        if (!NIL_P(byte_order))
                g_data_input_stream_set_byte_order(_SELF(self),
                                                   RVAL2GDATASTREAMBYTEORDER(byte_order));

        if (!NIL_P(newline_type))
                g_data_input_stream_set_newline_type(_SELF(self),
                                                     RVAL2GDATASTREAMNEWLINETYPE(newline_type));

        return Qnil;
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self) \
        { \
                return type(g_data_input_stream_get_##name(_SELF(self))); \
        }

#define DEFSETMETHOD(name, type) \
        DECLARE(name)(VALUE self, VALUE value) \
        { \
                g_data_input_stream_##name(_SELF(self), type(value)); \
                return self; \
        }

DEFGETMETHOD(byte_order, GDATASTREAMBYTEORDER2RVAL)
DEFSETMETHOD(set_byte_order, RVAL2GDATASTREAMBYTEORDER)
DEFGETMETHOD(newline_type, GDATASTREAMNEWLINETYPE2RVAL)
DEFSETMETHOD(set_newline_type, RVAL2GDATASTREAMNEWLINETYPE)

#define DEFREADMETHOD(name, c_type, type) \
        DECLARE_VARG(read_##name) \
        { \
                GCancellable *cancellable; \
                GError *error = NULL; \
                c_type value; \
                SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE); \
                value = g_data_input_stream_read_##name(_SELF(self), \
                                                        cancellable, \
                                                        &error); \
                if (error != NULL) \
                        rbgio_raise_io_error(error); \
                return type(value); \
        }

DEFREADMETHOD(byte, guchar, UINT2NUM)
DEFREADMETHOD(int16, gint16, INT2FIX)
DEFREADMETHOD(uint16, guint16, UINT2NUM)
DEFREADMETHOD(int32, gint32, INT2NUM)
DEFREADMETHOD(uint32, guint32, UINT2NUM)
DEFREADMETHOD(int64, gint64, rbglib_int64_to_num)
DEFREADMETHOD(uint64, guint64, rbglib_uint64_to_num)

DECLARE_VARG(read_line)
{
        gsize length;
        GCancellable *cancellable;
        GError *error;
        char *line;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        line = g_data_input_stream_read_line(_SELF(self),
                                             &length,
                                             cancellable,
                                             &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        if (line == NULL)
                return Qnil;

        return CSTR2RVAL_OWN(line, length);
}

DECLARE_VARG(read_line_async)
{
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_data_input_stream_read_line_async(_SELF(self),
                                            io_priority,
                                            cancellable,
                                            rbgio_async_ready_callback,
                                            (gpointer)block);

        return self;
}

DECLARE(read_line_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        gsize length;
        char *line;

        line = g_data_input_stream_read_line_finish(_SELF(self),
                                                    RVAL2GASYNCRESULT(result),
                                                    &length,
                                                    &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return CSTR2RVAL_OWN(line, length);
}

DECLARE_VARG(read_until)
{
        const char *stop_chars;
        gsize length;
        GCancellable *cancellable;
        GError *error;
        char *string;

        SCAN_2_ARGS("11",
                    stop_chars, RVAL2CSTR,
                    cancellable, RVAL2GCANCELLABLE);

        string = g_data_input_stream_read_until(_SELF(self),
                                                stop_chars,
                                                &length,
                                                cancellable,
                                                &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return CSTR2RVAL_OWN(string, length);
}

DECLARE_VARG(read_until_async)
{
        const char *stop_chars;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("12",
                               stop_chars, RVAL2CSTR,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_data_input_stream_read_until_async(_SELF(self),
                                             stop_chars,
                                             io_priority,
                                             cancellable,
                                             rbgio_async_ready_callback,
                                             (gpointer)block);

        return self;
}

DECLARE(read_until_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        gsize length;
        char *string;

        string = g_data_input_stream_read_until_finish(_SELF(self),
                                                       RVAL2GASYNCRESULT(result),
                                                       &length,
                                                       &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return CSTR2RVAL_OWN(string, length);
}

void
Init_gdatainputstream(VALUE glib)
{
        VALUE datainputstream = G_DEF_CLASS(G_TYPE_DATA_INPUT_STREAM,
                                            "DataInputStream",
                                            glib);

        DEF_METHOD(datainputstream, initialize, -1);
        DEF_ATTRIBUTE(datainputstream, byte_order);
        DEF_ATTRIBUTE(datainputstream, newline_type);
        DEF_METHOD(datainputstream, read_byte, -1);
        DEF_METHOD(datainputstream, read_int16, -1);
        DEF_METHOD(datainputstream, read_uint16, -1);
        DEF_METHOD(datainputstream, read_int32, -1);
        DEF_METHOD(datainputstream, read_uint32, -1);
        DEF_METHOD(datainputstream, read_int64, -1);
        DEF_METHOD(datainputstream, read_uint64, -1);
        DEF_METHOD(datainputstream, read_line, -1);
        DEF_METHOD(datainputstream, read_line_async, -1);
        DEF_METHOD(datainputstream, read_line_finish, 1);
        DEF_METHOD(datainputstream, read_until, -1);
        DEF_METHOD(datainputstream, read_until_async, -1);
        DEF_METHOD(datainputstream, read_until_finish, 1);
}
