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
#include <gio/gunixfdmessage.h>

#define RG_TARGET_NAMESPACE cUnixFDMessage
#define _SELF(value) G_UNIX_FD_MESSAGE(RVAL2GOBJ(value))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE fd_list;
        GSocketControlMessage *message;

        rb_scan_args(argc, argv, "01", &fd_list);
        message = NIL_P(fd_list) ?
                g_unix_fd_message_new() :
                g_unix_fd_message_new_with_fd_list(RVAL2GUNIXFDLIST(fd_list));
        G_INITIALIZE(self, message);

        return Qnil;
}

static VALUE
rg_append_fd(VALUE self, VALUE fd)
{
        GError *error = NULL;

        if (!g_unix_fd_message_append_fd(_SELF(self), RVAL2FD(fd), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_steal_fds(VALUE self)
{
        return GFDS2ARY_FREE(g_unix_fd_message_steal_fds(_SELF(self), NULL));
}
#endif

void
Init_gunixfdmessage(G_GNUC_UNUSED VALUE mGio)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_UNIX_FD_MESSAGE, "UnixFDMessage", mGio);

        RG_DEF_METHOD(initialize, -1);
        RG_DEF_METHOD(append_fd, 1);
        RG_DEF_ALIAS("<<", "append_fd");
        RG_DEF_METHOD(steal_fds, 0);
#endif
}
