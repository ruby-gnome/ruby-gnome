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

#define _SELF(value) RVAL2GFILEOUTPUTSTREAM(value)

static VALUE
stream_query_info(int argc, VALUE *argv, VALUE self)
{
        VALUE attributes, cancellable;
        GError *error = NULL;
        GFileInfo *info;

        rb_scan_args(argc, argv, "02", &attributes, &cancellable);
        info = g_file_output_stream_query_info(_SELF(self),
                                               RVAL2ATTRIBUTESDEFAULT(attributes),
                                               RVAL2GCANCELLABLE(cancellable),
                                               &error);
        if (info == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

static VALUE
stream_query_info_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbattributes, rbio_priority, rbcancellable, block;
        const char *attributes;
        int io_priority;
        GCancellable *cancellable;

	rb_scan_args(argc, argv, "03&", &rbattributes, &rbio_priority, &rbcancellable, &block);
	attributes = RVAL2ATTRIBUTESDEFAULT(rbattributes);
	io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
	cancellable = RVAL2GCANCELLABLE(rbcancellable);
	SAVE_BLOCK(block);

        g_file_output_stream_query_info_async(_SELF(self),
                                              attributes,
                                              io_priority,
                                              cancellable,
                                              rbgio_async_ready_callback,
                                              (gpointer)block);

        return self;
}

static VALUE
stream_query_info_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GFileInfo *info;

        info = g_file_output_stream_query_info_finish(_SELF(self),
                                                      RVAL2GASYNCRESULT(result),
                                                      &error);
        if (info == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

static VALUE
stream_etag(VALUE self)
{
        return CSTR2RVAL(g_file_output_stream_get_etag(_SELF(self)));
}

void
Init_gfileoutputstream(VALUE glib)
{
        VALUE fileoutputstream = G_DEF_CLASS(G_TYPE_FILE_OUTPUT_STREAM,
                                             "FileOutputStream",
                                             glib);

        rb_undef_alloc_func(fileoutputstream);

        rb_define_method(fileoutputstream, "query_info", stream_query_info, -1);
        rb_define_method(fileoutputstream, "query_info_async", stream_query_info_async, -1);
        rb_define_method(fileoutputstream, "query_info_finish", stream_query_info_finish, 1);
        rb_define_method(fileoutputstream, "etag", stream_etag, 0);
}
