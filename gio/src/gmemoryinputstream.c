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
        G_MEMORY_INPUT_STREAM(RVAL2GOBJ(value))

DECLARE(new_from_data)(VALUE self, VALUE data)
{
        StringValue(data);

        G_CHILD_ADD(self, data);

        return GOBJ2RVAL(g_memory_input_stream_new_from_data(RSTRING_PTR(data),
                                                             RSTRING_LEN(data),
                                                             NULL));
}

DECLARE(initialize)(VALUE self)
{
        G_INITIALIZE(self, g_memory_input_stream_new());

        return Qnil;
}

DECLARE(add_data)(VALUE self, VALUE data)
{
        StringValue(data);

        G_CHILD_ADD(self, data);

        g_memory_input_stream_add_data(_SELF(self),
                                       RSTRING_PTR(data),
                                       RSTRING_LEN(data),
                                       NULL);

        return self;
}

void
Init_gmemoryinputstream(VALUE glib)
{
        VALUE memoryinputstream = G_DEF_CLASS(G_TYPE_MEMORY_INPUT_STREAM,
                                              "MemoryInputStream",
                                              glib);

        DEF_SINGLETON_METHOD(memoryinputstream, new_from_data, 1);

        DEF_METHOD(memoryinputstream, initialize, 0);
        DEF_METHOD(memoryinputstream, add_data, 1);
}
