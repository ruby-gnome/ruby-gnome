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

#define _SELF(value) G_BUFFERED_OUTPUT_STREAM(RVAL2GOBJ(value))

static VALUE
bufferedoutputstream_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE rbbase_stream, size;
        GOutputStream *base_stream, *stream;

        rb_scan_args(argc, argv, "11", &rbbase_stream, &size);
        base_stream = RVAL2GOUTPUTSTREAM(rbbase_stream);

        stream = NIL_P(size) ?
                g_buffered_output_stream_new(base_stream) :
                g_buffered_output_stream_new_sized(base_stream, RVAL2GSIZE(size));
        G_INITIALIZE(self, stream);

        return Qnil;
}

void
Init_gbufferedoutputstream(VALUE glib)
{
        VALUE bufferedoutputstream = G_DEF_CLASS(G_TYPE_BUFFERED_OUTPUT_STREAM, "BufferedOutputStream", glib);

        rb_define_method(bufferedoutputstream, "initialize", bufferedoutputstream_initialize, -1);
}
