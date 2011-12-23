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

#define RG_TARGET_NAMESPACE cSocketConnection
#define _SELF(value) G_SOCKET_CONNECTION(RVAL2GOBJ(value))

static VALUE
rg_local_address(VALUE self)
{
        GError *error = NULL;
        GSocketAddress *address;

        address = g_socket_connection_get_local_address(_SELF(self), &error);
        if (address == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(address);
}

static VALUE
rg_remote_address(VALUE self)
{
        GError *error = NULL;
        GSocketAddress *address;

        address = g_socket_connection_get_remote_address(_SELF(self), &error);
        if (address == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(address);
}

/* TODO: lookup_type */

/* TODO: register_type */

void
Init_gsocketconnection(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE;

        RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_SOCKET_CONNECTION, "SocketConnection", mGio);

        RG_DEF_METHOD(local_address, 0);
        RG_DEF_METHOD(remote_address, 0);
}
