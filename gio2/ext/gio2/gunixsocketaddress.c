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

#include "gio2.h"

#ifdef HAVE_GIO_UNIX
#include <gio/gunixsocketaddress.h>

#define RG_TARGET_NAMESPACE cUnixSocketAddress
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
unixsocketaddress_abstract_names_supported(G_GNUC_UNUSED VALUE self)
{
        return CBOOL2RVAL(g_unix_socket_address_abstract_names_supported());
}
#endif

void
Init_gunixsocketaddress(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_UNIX_SOCKET_ADDRESS, "UnixSocketAddress", glib);

#  if GLIB_CHECK_VERSION(2, 26, 0)
        G_DEF_CLASS(G_TYPE_UNIX_SOCKET_ADDRESS_TYPE, "Type", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_UNIX_SOCKET_ADDRESS_TYPE, "G_UNIX_SOCKET_ADDRESS_");
#  endif

        rb_define_singleton_method(RG_TARGET_NAMESPACE, "new_abstract", unixsocketaddress_new_abstract, 1);
        rb_define_singleton_method(RG_TARGET_NAMESPACE, "abstract_names_supported?", unixsocketaddress_abstract_names_supported, 0);

        rb_define_method(RG_TARGET_NAMESPACE, "initialize", unixsocketaddress_initialize, 1);
#endif
}
