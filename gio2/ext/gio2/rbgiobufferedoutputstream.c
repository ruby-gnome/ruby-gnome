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

#define RG_TARGET_NAMESPACE cBufferedOutputStream
#define _SELF(value) G_BUFFERED_OUTPUT_STREAM(RVAL2GOBJ(value))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
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
Init_gbufferedoutputstream(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_BUFFERED_OUTPUT_STREAM, "BufferedOutputStream", mGio);

        RG_DEF_METHOD(initialize, -1);
}
