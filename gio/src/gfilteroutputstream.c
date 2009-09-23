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
        G_FILTER_OUTPUT_STREAM(RVAL2GOBJ(value))

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self) \
        { \
                return type(g_filter_output_stream_get_##name(_SELF(self))); \
        }

DEFGETMETHOD(base_stream, GOBJ2RVAL)
DEFGETMETHOD(close_base_stream, CBOOL2RVAL)

DECLARE(set_close_base_stream)(VALUE self, VALUE close_base)
{
        g_filter_output_stream_set_close_base_stream(_SELF(self),
                                                     RVAL2CBOOL(close_base));

        return self;
}

void
Init_gfilteroutputstream(VALUE glib)
{
        VALUE filteroutputstream = G_DEF_CLASS(G_TYPE_FILTER_OUTPUT_STREAM,
                                               "FilterOutputStream",
                                               glib);

        rb_undef_alloc_func(filteroutputstream);

        DEF_METHOD(filteroutputstream, base_stream, 0);
        DEF_BOOL_METHOD(filteroutputstream, close_base_stream, 0);
        DEF_SET_METHOD(filteroutputstream, close_base_stream);
}
