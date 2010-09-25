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
#include <gio/gunixfdlist.h>

#define _SELF(value) RVAL2GUNIXFDLIST(value)

static VALUE
unixfdlist_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE rbfds;
        gint i, n_fds;
        gint *fds;

        rb_scan_args(argc, argv, "0*", &rbfds);
        n_fds = RARRAY_LEN(rbfds);
        if (n_fds == 0) {
                G_INITIALIZE(self, g_unix_fd_list_new());
                return Qnil;
        }

        fds = ALLOCA_N(gint, n_fds);
        for (i = 0; i < n_fds; i++)
                fds[0] = RVAL2FD(RARRAY_PTR(rbfds)[i]);

        G_INITIALIZE(self, g_unix_fd_list_new_from_array(fds, n_fds));

        return Qnil;
}

static VALUE
unixfdlist_get_length(VALUE self)
{
        return GINT2RVAL(g_unix_fd_list_get_length(_SELF(self)));
}

static VALUE
unixfdlist_get(VALUE self, VALUE index)
{
        GError *error = NULL;
        gint fd;

        fd = g_unix_fd_list_get(_SELF(self), RVAL2GINT(index), &error);
        if (fd == -1)
                rbgio_raise_error(error);

        /* TODO: This fd must be closed properly.  How do we deal with that? */
        return FD2RVAL(fd);
}

static VALUE
unixfdlist_peek_fds(VALUE self)
{
        return GFDS2ARY(g_unix_fd_list_peek_fds(_SELF(self), NULL));
}

static VALUE
unixfdlist_steal_fds(VALUE self)
{
        return GFDS2ARY_FREE(g_unix_fd_list_steal_fds(_SELF(self), NULL));
}

static VALUE
unixfdlist_append(VALUE self, VALUE fd)
{
        GError *error = NULL;
        int index;

        index = g_unix_fd_list_append(_SELF(self), RVAL2FD(fd), &error);
        if (index == -1)
                rbgio_raise_error(error);

        return self;
}
#endif

void
Init_gunixfdlist(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE unixfdlist = G_DEF_CLASS(G_TYPE_UNIX_FD_LIST, "UnixFdList", glib);

        rb_define_method(unixfdlist, "initialize", unixfdlist_initialize, -1);
        rb_define_method(unixfdlist, "length", unixfdlist_get_length, 0);
        rb_define_method(unixfdlist, "get", unixfdlist_get, 1);
        rb_define_alias(unixfdlist, "[]", "get");
        rb_define_method(unixfdlist, "peek_fds", unixfdlist_peek_fds, 0);
        rb_define_method(unixfdlist, "steal_fds", unixfdlist_steal_fds, 0);
        rb_define_method(unixfdlist, "append", unixfdlist_append, 1);
        rb_define_alias(unixfdlist, "<<", "append");
#endif
}
