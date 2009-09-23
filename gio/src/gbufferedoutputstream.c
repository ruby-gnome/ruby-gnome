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
        G_BUFFERED_OUTPUT_STREAM(RVAL2GOBJ(value))

DECLARE_VARG(initialize)
{
        VALUE rbbase_stream,
              size;
        GOutputStream *base_stream;
        GOutputStream *stream;

        rb_scan_args(argc, argv, "11", &rbbase_stream, &size);
        base_stream = RVAL2GOUTPUTSTREAM(rbbase_stream);

        if (NIL_P(size))
                stream = g_buffered_output_stream_new(base_stream);
        else
                stream = g_buffered_output_stream_new_sized(base_stream,
                                                            NUM2UINT(size));

        G_INITIALIZE(self, stream);

        return Qnil;
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self) \
        { \
                return type(g_buffered_output_stream_get_##name(_SELF(self))); \
        }

#define DEFSETMETHOD(name, type) \
        DECLARE(name)(VALUE self, VALUE value) \
        { \
                g_buffered_output_stream_##name(_SELF(self), type(value)); \
                return self; \
        }

DEFGETMETHOD(buffer_size, UINT2NUM)
DEFSETMETHOD(set_buffer_size, NUM2UINT)
DEFGETMETHOD(auto_grow, CBOOL2RVAL)
DEFSETMETHOD(set_auto_grow, RVAL2CBOOL)

void
Init_gbufferedoutputstream(VALUE glib)
{
        VALUE bufferedoutputstream = G_DEF_CLASS(G_TYPE_BUFFERED_OUTPUT_STREAM,
                                                 "BufferedOutputStream",
                                                 glib);

        DEF_METHOD(bufferedoutputstream, initialize, -1);
        DEF_ATTRIBUTE(bufferedoutputstream, buffer_size);
        DEF_ATTRIBUTE(bufferedoutputstream, auto_grow);
}
