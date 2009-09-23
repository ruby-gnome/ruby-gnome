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
        G_LOADABLE_ICON(RVAL2GOBJ(value))

DECLARE_VARG(load)
{
        int size;
        char *type;
        GCancellable *cancellable;
        GError *error = NULL;
        GInputStream *stream;

        SCAN_2_ARGS("11", size, NUM2INT, cancellable, RVAL2GCANCELLABLE);

        stream = g_loadable_icon_load(_SELF(self),
                                      size,
                                      &type,
                                      cancellable,
                                      &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return rb_assoc_new(GOBJ2RVAL(stream), CSTR2RVAL(type));
}

DECLARE_VARG(load_async)
{
        int size;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("11",
                               size, NUM2INT,
                               cancellable, RVAL2GCANCELLABLE);

        g_loadable_icon_load_async(_SELF(self),
                                   size,
                                   cancellable,
                                   rbgio_async_ready_callback,
                                   (gpointer)block);

        return self;
}

DECLARE(load_finish)(VALUE self, VALUE result)
{
        char *type;
        GError *error = NULL;
        GInputStream *stream;

        stream = g_loadable_icon_load_finish(_SELF(self),
                                             RVAL2GASYNCRESULT(result),
                                             &type,
                                             &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return rb_assoc_new(GOBJ2RVAL(stream), CSTR2RVAL_FREE(type));
}

void
Init_gloadableicon(VALUE glib)
{
        VALUE loadableicon = G_DEF_INTERFACE(G_TYPE_LOADABLE_ICON,
                                             "LoadableIcon",
                                             glib);

        DEF_METHOD(loadableicon, load, -1);
        DEF_METHOD(loadableicon, load_async, -1);
        DEF_METHOD(loadableicon, load_finish, 1);
}
