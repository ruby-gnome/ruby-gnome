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

#define _SELF(value) G_SOCKET_CONNECTION(RVAL2GOBJ(value))

static VALUE
socketconnection_get_local_address(VALUE self)
{
        GError *error = NULL;
        GSocketAddress *address;

        address = g_socket_connection_get_local_address(_SELF(self), &error);
        if (address == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(address);
}

static VALUE
socketconnection_get_remote_address(VALUE self)
{
        GError *error = NULL;
        GSocketAddress *address;

        address = g_socket_connection_get_remote_address(_SELF(self), &error);
        if (address == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(address);
}

static VALUE
socketconnectionfactory_create_connection(G_GNUC_UNUSED VALUE self, VALUE socket)
{
        return GOBJ2RVAL_UNREF(g_socket_connection_factory_create_connection(RVAL2GSOCKET(socket)));
}

/* TODO: lookup_type */

/* TODO: register_type */

void
Init_gsocketconnection(VALUE glib)
{
        VALUE socketconnection, socketconnectionfactory;

        socketconnection = G_DEF_CLASS(G_TYPE_SOCKET_CONNECTION, "SocketConnection", glib);

        rb_define_method(socketconnection, "local_address", socketconnection_get_local_address, 0);
        rb_define_method(socketconnection, "remote_address", socketconnection_get_remote_address, 0);

        /* TODO: Perhaps just move this to SocketConnection? */
        socketconnectionfactory = rb_define_module_under(glib, "SocketConnectionFactory");

        rb_define_module_function(socketconnectionfactory, "create_connection", socketconnectionfactory_create_connection, 1);
}
