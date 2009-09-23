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
        RVAL2GFILEINPUTSTREAM(value)

DECLARE_VARG(query_info)
{
        const char *attributes;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileInfo *info;

        SCAN_2_ARGS("02",
                    attributes, RVAL2ATTRIBUTESDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        info = g_file_input_stream_query_info(_SELF(self),
                                              attributes,
                                              cancellable,
                                              &error);
        if (info == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

DECLARE_VARG(query_info_async)
{
        const char *attributes;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("03",
                               attributes, RVAL2ATTRIBUTESDEFAULT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_input_stream_query_info_async(_SELF(self),
                                             attributes,
                                             io_priority,
                                             cancellable,
                                             rbgio_async_ready_callback,
                                             (gpointer)block);

        return self;
}

DECLARE(query_info_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        GFileInfo *info;

        info = g_file_input_stream_query_info_finish(_SELF(self),
                                                     RVAL2GASYNCRESULT(result),
                                                     &error);
        if (info == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

void
Init_gfileinputstream(VALUE glib)
{
        VALUE fileinputstream = G_DEF_CLASS(G_TYPE_FILE_INPUT_STREAM,
                                            "FileInputStream",
                                            glib);

        rb_undef_alloc_func(fileinputstream);

        DEF_METHOD(fileinputstream, query_info, -1);
        DEF_METHOD(fileinputstream, query_info_async, -1);
        DEF_METHOD(fileinputstream, query_info_finish, 1);
}
