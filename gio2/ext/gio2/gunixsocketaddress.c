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
#include <gio/gunixsocketaddress.h>

#define _SELF(value) G_UNIX_SOCKET_ADDRESS(RVAL2GOBJ(value))

static VALUE
unixsocketaddress_initialize(VALUE self, VALUE path)
{
        G_INITIALIZE(self, g_unix_socket_address_new(RVAL2CSTR(path)));

        return Qnil;
}

static VALUE
unixsocketaddress_new_abstract(G_GNUC_UNUSED VALUE self, VALUE path)
{
        StringValue(path);
        return GOBJ2RVAL_UNREF(g_unix_socket_address_new_abstract(RSTRING_PTR(path),
                                                                  RSTRING_LEN(path)));
}

static VALUE
unixsocketaddress_get_is_abstract(VALUE self)
{
        return CBOOL2RVAL(g_unix_socket_address_get_is_abstract(_SELF(self)));
}

static VALUE
unixsocketaddress_get_path(VALUE self)
{
        return rb_str_new(g_unix_socket_address_get_path(_SELF(self)),
                          g_unix_socket_address_get_path_len(_SELF(self)));
}

/* TODO: get_path_len is pointless in Ruby, right? */

static VALUE
unixsocketaddress_abstract_names_supported(G_GNUC_UNUSED VALUE self)
{
        return CBOOL2RVAL(g_unix_socket_address_abstract_names_supported());
}
#endif

void
Init_gunixsocketaddress(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE unixsocketaddress = G_DEF_CLASS(G_TYPE_UNIX_SOCKET_ADDRESS, "UnixSocketAddress", glib);

        G_DEF_CLASS(G_TYPE_UNIX_SOCKET_ADDRESS_TYPE, "Type", unixsocketaddress);
        G_DEF_CONSTANTS(unixsocketaddress, G_TYPE_UNIX_SOCKET_ADDRESS_TYPE, "G_UNIX_SOCKET_ADDRESS_");

        rb_define_singleton_method(unixsocketaddress, "new_abstract", unixsocketaddress_new_abstract, 1);
        rb_define_singleton_method(unixsocketaddress, "abstract_names_supported?", unixsocketaddress_abstract_names_supported, 0);

        rb_define_method(unixsocketaddress, "initialize", unixsocketaddress_initialize, 1);
        rb_define_method(unixsocketaddress, "abstract?", unixsocketaddress_get_is_abstract, 0);
        rb_define_method(unixsocketaddress, "path", unixsocketaddress_get_path, 0);
#endif
}
