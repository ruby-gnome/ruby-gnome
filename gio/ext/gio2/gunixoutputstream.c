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

#ifdef HAVE_GIO_UNIX
#include <gio/gunixoutputstream.h>

#define _SELF(value) G_UNIX_OUTPUT_STREAM(RVAL2GOBJ(value))

static VALUE
unixoutputstream_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE fd, close_fd;

        rb_scan_args(argc, argv, "11", &fd, &close_fd);
        G_INITIALIZE(self, g_unix_output_stream_new(RVAL2FD(fd),
                                                    RVAL2CBOOL(close_fd)));

        return Qnil;
}

static VALUE
unixoutputstream_set_close_fd(VALUE self, VALUE close_fd)
{
        g_unix_output_stream_set_close_fd(_SELF(self), RVAL2CBOOL(close_fd));

        return self;
}

static VALUE
unixoutputstream_get_close_fd(VALUE self)
{
        return CBOOL2RVAL(g_unix_output_stream_get_close_fd(_SELF(self)));
}

static VALUE
unixoutputstream_get_fd(VALUE self)
{
        return FD2RVAL(g_unix_output_stream_get_fd(_SELF(self)));
}
#endif

void
Init_gunixoutputstream(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE unixoutputstream = G_DEF_CLASS(G_TYPE_UNIX_OUTPUT_STREAM, "UnixOutputStream", glib);

        rb_define_method(unixoutputstream, "initialize", unixoutputstream_initialize, -1);
        rb_define_method(unixoutputstream, "set_close_fd", unixoutputstream_set_close_fd, 1);
        G_DEF_SETTER(unixoutputstream, "close_fd");
        rb_define_method(unixoutputstream, "close_fd?", unixoutputstream_get_close_fd, 0);
        rb_define_method(unixoutputstream, "fd", unixoutputstream_get_fd, 0);
#endif
}
