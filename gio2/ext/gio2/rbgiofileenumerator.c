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

#define RG_TARGET_NAMESPACE cFileEnumerator
#define _SELF(value) RVAL2GFILEENUMERATOR(value)

static VALUE
rg_next_file(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        GFileInfo *info;

        rb_scan_args(argc, argv, "01", &cancellable);
        info = g_file_enumerator_next_file(_SELF(self),
                                           RVAL2GCANCELLABLE(cancellable),
                                           &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

static VALUE
rg_close(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "01", &cancellable);
        if (!g_file_enumerator_close(_SELF(self), RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_next_files_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbnum_files, rbio_priority, rbcancellable, block;
        int num_files;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "12&", &rbnum_files, &rbio_priority, &rbcancellable, &block);
        num_files = NUM2INT(rbnum_files);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_enumerator_next_files_async(_SELF(self),
                                           num_files,
                                           io_priority,
                                           cancellable,
                                           rbgio_async_ready_callback,
                                           (gpointer)block);

        return self;
}

static VALUE
rg_next_files_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GList *files = g_file_enumerator_next_files_finish(_SELF(self),
                                                           RVAL2GASYNCRESULT(result),
                                                           &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GLIST2ARY_FREE(files);
}

static VALUE
rg_close_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "02&", &rbio_priority, &rbcancellable, &block);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_enumerator_close_async(_SELF(self),
                                      io_priority,
                                      cancellable,
                                      rbgio_async_ready_callback,
                                      (gpointer)block);

        return self;
}

static VALUE
rg_close_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_file_enumerator_close_finish(_SELF(self),
                                            RVAL2GASYNCRESULT(result),
                                            &error))
                rbgio_raise_error(error);

        return Qnil;
}

static VALUE
rg_closed_p(VALUE self)
{
        return CBOOL2RVAL(g_file_enumerator_is_closed(_SELF(self)));
}

static VALUE
rg_has_pending_p(VALUE self)
{
        return CBOOL2RVAL(g_file_enumerator_has_pending(_SELF(self)));
}

static VALUE
rg_set_pending(VALUE self, VALUE pending)
{
        g_file_enumerator_set_pending(_SELF(self), RVAL2CBOOL(pending));

        return self;
}

static VALUE
rg_container(VALUE self)
{
        return GOBJ2RVAL(g_file_enumerator_get_container(_SELF(self)));
}

void
Init_gfileenumerator(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_FILE_ENUMERATOR, "FileEnumerator", mGio);

        RG_DEF_METHOD(next_file, -1);
        RG_DEF_METHOD(close, -1);
        RG_DEF_METHOD(next_files_async, -1);
        RG_DEF_METHOD(next_files_finish, 1);
        RG_DEF_METHOD(close_async, -1);
        RG_DEF_METHOD(close_finish, 1);
        RG_DEF_METHOD_P(closed, 0);
        RG_DEF_METHOD_P(has_pending, 0);
        RG_DEF_METHOD(set_pending, 1);
        RG_DEF_METHOD(container, 0);
}
