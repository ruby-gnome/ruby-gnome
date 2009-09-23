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
        G_SEEKABLE(RVAL2GOBJ(value))

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_seekable_##name(_SELF(self))); }

DEFSIMPLEMETHOD(tell, rbglib_int64_to_num)
DEFSIMPLEMETHOD(can_seek, CBOOL2RVAL)

DECLARE_VARG(seek)
{
        gint64 offset;
        GSeekType type;
        GCancellable *cancellable;
        GError *error = NULL;

        SCAN_3_ARGS("12",
                    offset, rbglib_num_to_int64,
                    type, RVAL2GSEEKTYPEDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        if (!g_seekable_seek(_SELF(self),
                             offset,
                             type,
                             cancellable,
                             &error))
                rbgio_raise_io_error(error);

        return self;
}

DEFSIMPLEMETHOD(can_truncate, CBOOL2RVAL)

DECLARE_VARG(truncate)
{
        gint64 offset;
        GCancellable *cancellable;
        GError *error = NULL;

        SCAN_2_ARGS("11",
                    offset, rbglib_num_to_int64,
                    cancellable, RVAL2GCANCELLABLE);

        if (!g_seekable_truncate(_SELF(self), offset, cancellable, &error))
                rbgio_raise_io_error(error);

        return self;
}

void
Init_gseekable(VALUE glib)
{
        VALUE seekable = G_DEF_INTERFACE(G_TYPE_SEEKABLE, "Seekable", glib);

        DEF_METHOD(seekable, tell, 0);
        DEF_BOOL_METHOD(seekable, can_seek, 0);
        DEF_METHOD(seekable, seek, -1);
        DEF_BOOL_METHOD(seekable, can_truncate, 0);
        DEF_METHOD(seekable, truncate, -1);
}
