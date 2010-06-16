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

#define _SELF(value) G_SEEKABLE(RVAL2GOBJ(value))

static VALUE
seekable_tell(VALUE self)
{
	return rbglib_int64_to_num(g_seekable_tell(_SELF(self)));
}

static VALUE
seekable_can_seek(VALUE self)
{
	return CBOOL2RVAL(g_seekable_can_seek(_SELF(self)));
}


static VALUE
seekable_seek(int argc, VALUE *argv, VALUE self)
{
        VALUE offset, type, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "12", &offset, &type, &cancellable);
        if (!g_seekable_seek(_SELF(self),
                             rbglib_num_to_int64(offset),
                             RVAL2GSEEKTYPEDEFAULT(type),
                             RVAL2GCANCELLABLE(cancellable),
                             &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
seekable_can_truncate(VALUE self)
{
	return CBOOL2RVAL(g_seekable_can_truncate(_SELF(self)));
}

static VALUE
seekable_truncate(int argc, VALUE *argv, VALUE self)
{
        VALUE offset, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &offset, &cancellable);
        if (!g_seekable_truncate(_SELF(self), rbglib_num_to_int64(offset),
                                 RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_io_error(error);

        return self;
}

void
Init_gseekable(VALUE glib)
{
        VALUE seekable = G_DEF_INTERFACE(G_TYPE_SEEKABLE, "Seekable", glib);

        rb_define_method(seekable, "tell", seekable_tell, 0);
        rb_define_method(seekable, "can_seek?", seekable_can_seek, 0);
        rb_define_method(seekable, "seek", seekable_seek, -1);
        rb_define_method(seekable, "can_truncate?", seekable_can_truncate, 0);
        rb_define_method(seekable, "truncate", seekable_truncate, -1);
}
