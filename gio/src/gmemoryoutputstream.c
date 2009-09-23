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
        G_MEMORY_OUTPUT_STREAM(RVAL2GOBJ(value))

#if 0
DECLARE_VARG(initialize)
{
        G_INITIALIZE(self, g_memory_output_stream_new(…));

        return Qnil;
}
#endif

void
Init_gmemoryoutputstream(VALUE glib)
{
        VALUE memoryoutputstream = G_DEF_CLASS(G_TYPE_MEMORY_OUTPUT_STREAM,
                                               "MemoryOutputStream",
                                               glib);

        /* TODO: This needs work. */
     /*   DEF_METHOD(memoryoutputstream, initialize, -1); */
}
