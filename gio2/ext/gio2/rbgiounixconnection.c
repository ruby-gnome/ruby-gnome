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

#ifdef HAVE_GIO_UNIX
#include <gio/gunixconnection.h>

#define RG_TARGET_NAMESPACE cUnixConnection
#define _SELF(value) G_UNIX_CONNECTION(RVAL2GOBJ(value))

static VALUE
rg_receive_fd(int argc, VALUE *argv, VALUE self)
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
rg_send_fd(int argc, VALUE *argv, VALUE self)
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
Init_gunixconnection(G_GNUC_UNUSED VALUE mGio)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_UNIX_CONNECTION, "UnixConnection", mGio);

        RG_DEF_METHOD(receive_fd, -1);
        RG_DEF_METHOD(send_fd, -1);
#endif
}
