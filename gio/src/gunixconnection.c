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
#include <gio/gunixconnection.h>

#define _SELF(value) G_UNIX_CONNECTION(RVAL2GOBJ(value))

static VALUE
unixconnection_receive_fd(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        gint fd;

        rb_scan_args(argc, argv, "01", &cancellable);
        fd = g_unix_connection_receive_fd(_SELF(self),
                                          RVAL2GCANCELLABLE(cancellable),
                                          &error);
        if (fd == -1)
                rbgio_raise_error(error);

        return FD2RVAL(fd);
}

static VALUE
unixconnection_send_fd(int argc, VALUE *argv, VALUE self)
{
        VALUE fd, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &fd, &cancellable);
        if (!g_unix_connection_send_fd(_SELF(self), RVAL2FD(fd),
                                       RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_error(error);

        return self;
}
#endif

void
Init_gunixconnection(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE unixconnection = G_DEF_CLASS(G_TYPE_UNIX_CONNECTION, "UnixConnection", glib);

        rb_define_method(unixconnection, "receive_fd", unixconnection_receive_fd, -1);
        rb_define_method(unixconnection, "send_fd", unixconnection_send_fd, -1);
#endif
}
