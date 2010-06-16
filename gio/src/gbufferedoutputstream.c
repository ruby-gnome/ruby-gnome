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

#define _SELF(value) G_BUFFERED_OUTPUT_STREAM(RVAL2GOBJ(value))

static VALUE
stream_initialize(int argc, VALUE *argv, VALUE self)
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

static VALUE
stream_buffer_size(VALUE self)
{
	return UINT2NUM(g_buffered_output_stream_get_buffer_size(_SELF(self)));
}

static VALUE
stream_set_buffer_size(VALUE self, VALUE size)
{
        g_buffered_output_stream_set_buffer_size(_SELF(self), NUM2UINT(size));

        return self;
}

static VALUE
stream_auto_grow(VALUE  self)
{
	return CBOOL2RVAL(g_buffered_output_stream_get_auto_grow(_SELF(self)));
}

static VALUE
stream_set_auto_grow(VALUE self, VALUE grow)
{
        g_buffered_output_stream_set_auto_grow(_SELF(self), RVAL2CBOOL(grow));

        return self;
}

void
Init_gbufferedoutputstream(VALUE glib)
{
        VALUE bufferedoutputstream = G_DEF_CLASS(G_TYPE_BUFFERED_OUTPUT_STREAM, "BufferedOutputStream", glib);

        rb_define_method(bufferedoutputstream, "initialize", stream_initialize, -1);
        rb_define_method(bufferedoutputstream, "buffer_size", stream_buffer_size, 0);
	rb_define_method(bufferedoutputstream, "set_buffer_size", stream_set_buffer_size, 1);
	G_DEF_SETTER(bufferedoutputstream, "buffer_size");
        rb_define_method(bufferedoutputstream, "auto_grow", stream_auto_grow, 0);
	rb_define_method(bufferedoutputstream, "set_auto_grow", stream_set_auto_grow, 1);
	G_DEF_SETTER(bufferedoutputstream, "auto_grow");
}
