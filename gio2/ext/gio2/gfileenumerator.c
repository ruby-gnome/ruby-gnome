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

#define _SELF(value) G_FILE_ENUMERATOR(RVAL2GOBJ(value))

static VALUE
fileenumerator_next_file(int argc, VALUE *argv, VALUE self)
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
fileenumerator_close(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "01", &cancellable);
        if (!g_file_enumerator_close(_SELF(self), RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
fileenumerator_next_files_async(int argc, VALUE *argv, VALUE self)
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
fileenumerator_next_files_finish(VALUE self, VALUE result)
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
fileenumerator_close_async(int argc, VALUE *argv, VALUE self)
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
fileenumerator_close_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_file_enumerator_close_finish(_SELF(self),
                                            RVAL2GASYNCRESULT(result),
                                            &error))
                rbgio_raise_error(error);

        return Qnil;
}

static VALUE
fileenumerator_is_closed(VALUE self)
{
        return CBOOL2RVAL(g_file_enumerator_is_closed(_SELF(self)));
}

static VALUE
fileenumerator_has_pending(VALUE self)
{
        return CBOOL2RVAL(g_file_enumerator_has_pending(_SELF(self)));
}

static VALUE
fileenumerator_set_pending(VALUE self, VALUE pending)
{
        g_file_enumerator_set_pending(_SELF(self), RVAL2CBOOL(pending));

        return self;
}

static VALUE
fileenumerator_get_container(VALUE self)
{
        return GOBJ2RVAL(g_file_enumerator_get_container(_SELF(self)));
}

void
Init_gfileenumerator(VALUE glib)
{
        VALUE fileenumerator = G_DEF_CLASS(G_TYPE_FILE_ENUMERATOR, "FileEnumerator", glib);

        rb_define_method(fileenumerator, "next_file", fileenumerator_next_file, -1);
        rb_define_method(fileenumerator, "close", fileenumerator_close, -1);
        rb_define_method(fileenumerator, "next_files_async", fileenumerator_next_files_async, -1);
        rb_define_method(fileenumerator, "next_files_finish", fileenumerator_next_files_finish, 1);
        rb_define_method(fileenumerator, "close_async", fileenumerator_close_async, -1);
        rb_define_method(fileenumerator, "close_finish", fileenumerator_close_finish, 1);
        rb_define_method(fileenumerator, "closed?", fileenumerator_is_closed, 0);
        rb_define_method(fileenumerator, "has_pending?", fileenumerator_has_pending, 0);
        rb_define_method(fileenumerator, "set_pending", fileenumerator_set_pending, 1);
        G_DEF_SETTER(fileenumerator, "pending");
        rb_define_method(fileenumerator, "container", fileenumerator_get_container, 0);
}
