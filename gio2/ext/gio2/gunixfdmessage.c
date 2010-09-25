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
#include <gio/gunixfdmessage.h>

#define _SELF(value) G_UNIX_FD_MESSAGE(RVAL2GOBJ(value))

static VALUE
unixfdmessage_initialize(int argc, VALUE *argv, VALUE self)
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
unixfdmessage_get_fd_list(VALUE self)
{
        /* TODO: Should we relate the return value with self?  It’s apparently
         * valid as long as self is valid. */
        return GOBJ2RVAL(g_unix_fd_message_get_fd_list(_SELF(self)));
}

static VALUE
unixfdmessage_append_fd(VALUE self, VALUE fd)
{
        GError *error = NULL;

        if (!g_unix_fd_message_append_fd(_SELF(self), RVAL2FD(fd), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
unixfdmessage_steal_fds(VALUE self)
{
        return GFDS2ARY_FREE(g_unix_fd_message_steal_fds(_SELF(self), NULL));
}
#endif

void
Init_gunixfdmessage(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE unixfdmessage = G_DEF_CLASS(G_TYPE_UNIX_FD_MESSAGE, "UnixFDMessage", glib);

        rb_define_method(unixfdmessage, "initialize", unixfdmessage_initialize, -1);
        rb_define_method(unixfdmessage, "fd_list", unixfdmessage_get_fd_list, 0);
        rb_define_method(unixfdmessage, "append_fd", unixfdmessage_append_fd, 1);
        rb_define_alias(unixfdmessage, "<<", "append_fd");
        rb_define_method(unixfdmessage, "steal_fds", unixfdmessage_steal_fds, 0);
#endif
}
