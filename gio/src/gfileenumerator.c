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
        G_FILE_ENUMERATOR(RVAL2GOBJ(value))

DECLARE_VARG(next_file)
{
        GCancellable *cancellable;
        GError *error = NULL;
        GFileInfo *info;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        info = g_file_enumerator_next_file(_SELF(self),
                                           cancellable,
                                           &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

DECLARE_VARG(close)
{
        GCancellable *cancellable;
        GError *error = NULL;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        if (!g_file_enumerator_close(_SELF(self), cancellable, &error))
                rbgio_raise_io_error(error);

        return self;
}

DECLARE_VARG(next_files_async)
{
        int num_files;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("12",
                               num_files, NUM2INT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_enumerator_next_files_async(_SELF(self),
                                           num_files,
                                           io_priority,
                                           cancellable,
                                           rbgio_async_ready_callback,
                                           (gpointer)block);

        return self;
}

DECLARE(next_files_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        GList *files = g_file_enumerator_next_files_finish(_SELF(self),
                                                           RVAL2GASYNCRESULT(result),
                                                           &error);
        if (error != NULL)
                rbgio_raise_io_error(error);

        return GLIST2ARY_FREE(files);
}

DECLARE_VARG(close_async)
{
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_enumerator_close_async(_SELF(self),
                                      io_priority,
                                      cancellable,
                                      rbgio_async_ready_callback,
                                      (gpointer)block);

        return self;
}

DECLARE(close_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_file_enumerator_close_finish(_SELF(self),
                                            RVAL2GASYNCRESULT(result),
                                            &error))
                rbgio_raise_io_error(error);

        return Qnil;
}

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_file_enumerator_##name(_SELF(self))); }

DEFSIMPLEMETHOD(is_closed, CBOOL2RVAL)
DEFSIMPLEMETHOD(has_pending, CBOOL2RVAL)

DECLARE(set_pending)(VALUE self, VALUE pending)
{
        g_file_enumerator_set_pending(_SELF(self), RVAL2CBOOL(pending));

        return self;
}

DECLARE(container)(VALUE self)
{
        return GOBJ2RVAL(g_file_enumerator_get_container(_SELF(self)));
}

void
Init_gfileenumerator(VALUE glib)
{
        /* TODO: Define this under GLib::File? */
        VALUE fileenumerator = G_DEF_CLASS(G_TYPE_FILE_ENUMERATOR,
                                           "FileEnumerator",
                                           glib);

        rb_undef_alloc_func(fileenumerator);

        DEF_METHOD(fileenumerator, next_file, -1);
        DEF_METHOD(fileenumerator, close, -1);
        DEF_METHOD(fileenumerator, next_files_async, -1);
        DEF_METHOD(fileenumerator, next_files_finish, 1);
        DEF_METHOD(fileenumerator, close_async, -1);
        DEF_METHOD(fileenumerator, close_finish, 1);
        DEF_IS_METHOD(fileenumerator, closed, 0);
        DEF_BOOL_METHOD(fileenumerator, has_pending, 0);
        DEF_SET_METHOD(fileenumerator, pending);
        DEF_METHOD(fileenumerator, container, 0);
}
