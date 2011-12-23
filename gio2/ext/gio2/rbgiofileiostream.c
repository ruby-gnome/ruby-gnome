/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE cFileIOStream
#define _SELF(value) G_FILE_IO_STREAM(RVAL2GOBJ(value))

static VALUE
rg_etag(VALUE self)
{
        return CSTR2RVAL(g_file_io_stream_get_etag(_SELF(self)));
}

static VALUE
rg_query_info(int argc, VALUE *argv, VALUE self)
{
        VALUE attributes, cancellable;
        GError *error = NULL;
        GFileInfo *info;

        rb_scan_args(argc, argv, "02", &attributes, &cancellable);
        info = g_file_io_stream_query_info(_SELF(self),
                                           RVAL2ATTRIBUTESDEFAULT(attributes),
                                           RVAL2GCANCELLABLE(cancellable),
                                           &error);
        if (info == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

static VALUE
rg_query_info_async(int argc, VALUE *argv, VALUE self)
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
        g_file_io_stream_query_info_async(_SELF(self),
                                          attributes,
                                          io_priority,
                                          cancellable,
                                          rbgio_async_ready_callback,
                                          (gpointer)block);

        return self;
}

static VALUE
rg_query_info_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GFileInfo *info;

        info = g_file_io_stream_query_info_finish(_SELF(self),
                                                  RVAL2GASYNCRESULT(result),
                                                  &error);
        if (info == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

void
Init_gfileiostream(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_FILE_IO_STREAM, "FileIOStream", mGio);

        RG_DEF_METHOD(etag, 0);
        RG_DEF_METHOD(query_info, -1);
        RG_DEF_METHOD(query_info_async, -1);
        RG_DEF_METHOD(query_info_finish, 1);
}
