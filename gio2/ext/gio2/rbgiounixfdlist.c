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
#include <gio/gunixfdlist.h>

#define RG_TARGET_NAMESPACE cUnixFDList
#define _SELF(value) RVAL2GUNIXFDLIST(value)

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE rbfds;
        long n;
        gint *fds;
        GUnixFDList *list;

        rb_scan_args(argc, argv, "0*", &rbfds);
        fds = RVAL2FDS(rbfds, n);
        if (n == 0) {
                G_INITIALIZE(self, g_unix_fd_list_new());
                return Qnil;
        }

        list = g_unix_fd_list_new_from_array(fds, n);

        g_free(fds);

        G_INITIALIZE(self, list);

        return Qnil;
}

static VALUE
rg_length(VALUE self)
{
        return GINT2RVAL(g_unix_fd_list_get_length(_SELF(self)));
}

static VALUE
rg_get(VALUE self, VALUE index)
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
rg_peek_fds(VALUE self)
{
        return GFDS2ARY(g_unix_fd_list_peek_fds(_SELF(self), NULL));
}

static VALUE
rg_steal_fds(VALUE self)
{
        return GFDS2ARY_FREE(g_unix_fd_list_steal_fds(_SELF(self), NULL));
}

static VALUE
rg_append(VALUE self, VALUE fd)
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
Init_gunixfdlist(G_GNUC_UNUSED VALUE mGio)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_UNIX_FD_LIST, "UnixFDList", mGio);

        RG_DEF_METHOD(initialize, -1);
        RG_DEF_METHOD(length, 0);
        RG_DEF_METHOD(get, 1);
        RG_DEF_ALIAS("[]", "get");
        RG_DEF_METHOD(peek_fds, 0);
        RG_DEF_METHOD(steal_fds, 0);
        RG_DEF_METHOD(append, 1);
        RG_DEF_ALIAS("<<", "append");
#endif
}
