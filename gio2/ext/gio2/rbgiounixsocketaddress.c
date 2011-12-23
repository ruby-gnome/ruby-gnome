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
#include <gio/gunixsocketaddress.h>

#define RG_TARGET_NAMESPACE cUnixSocketAddress
#define _SELF(value) G_UNIX_SOCKET_ADDRESS(RVAL2GOBJ(value))

static VALUE
rg_initialize(VALUE self, VALUE path)
{
        G_INITIALIZE(self, g_unix_socket_address_new(RVAL2CSTR(path)));

        return Qnil;
}

static VALUE
rg_s_new_abstract(G_GNUC_UNUSED VALUE self, VALUE path)
{
        StringValue(path);
        return GOBJ2RVAL_UNREF(g_unix_socket_address_new_abstract(RSTRING_PTR(path),
                                                                  RSTRING_LEN(path)));
}

static VALUE
rg_s_abstract_names_supported_p(G_GNUC_UNUSED VALUE self)
{
        return CBOOL2RVAL(g_unix_socket_address_abstract_names_supported());
}
#endif

void
Init_gunixsocketaddress(G_GNUC_UNUSED VALUE mGio)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_UNIX_SOCKET_ADDRESS, "UnixSocketAddress", mGio);

#  if GLIB_CHECK_VERSION(2, 26, 0)
        G_DEF_CLASS(G_TYPE_UNIX_SOCKET_ADDRESS_TYPE, "Type", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_UNIX_SOCKET_ADDRESS_TYPE, "G_UNIX_SOCKET_ADDRESS_");
#  endif

        RG_DEF_SMETHOD(new_abstract, 1);
        RG_DEF_SMETHOD_P(abstract_names_supported, 0);

        RG_DEF_METHOD(initialize, 1);
#endif
}
