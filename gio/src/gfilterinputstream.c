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

#define _SELF(value) G_FILTER_INPUT_STREAM(RVAL2GOBJ(value))

static VALUE
stream_base_stream(VALUE self)
{
	return GOBJ2RVAL(g_filter_input_stream_get_base_stream(_SELF(self)));
}

static VALUE
stream_close_base_stream(VALUE self)
{
	return CBOOL2RVAL(g_filter_input_stream_get_close_base_stream(_SELF(self)));
}

static VALUE
stream_set_close_base_stream(VALUE self, VALUE close_base)
{
        g_filter_input_stream_set_close_base_stream(_SELF(self),
                                                    RVAL2CBOOL(close_base));

        return self;
}

void
Init_gfilterinputstream(VALUE glib)
{
        VALUE filterinputstream = G_DEF_CLASS(G_TYPE_FILTER_INPUT_STREAM,
                                              "FilterInputStream",
                                              glib);

        rb_undef_alloc_func(filterinputstream);

        rb_define_method(filterinputstream, "base_stream", stream_base_stream, 0);
        rb_define_method(filterinputstream, "close_base_stream?", stream_close_base_stream, 0);
        rb_define_method(filterinputstream, "set_close_base_stream", stream_set_close_base_stream, 1);
	G_DEF_SETTER(filterinputstream, "close_base_stream");
}
