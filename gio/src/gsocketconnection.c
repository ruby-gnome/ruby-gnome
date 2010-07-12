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

#define _SELF(value) G_SOCKET_CONNECTION(RVAL2GOBJ(value))

static VALUE
socketconnection_get_remote_address(VALUE self)
{
        GError *error = NULL;
        GSocketAddress *address;

        address = g_socket_connection_get_remote_address(_SELF(self), &error);
        if (address == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL(address);
}

static VALUE
socketconnection_get_socket(VALUE self)
{
        return GOBJ2RVAL(g_socket_connection_get_socket(_SELF(self)));
}

static VALUE
socketconnectionfactory_create_connection(G_GNUC_UNUSED VALUE self, VALUE socket)
{
        return GOBJ2RVAL(g_socket_connection_factory_create_connection(RVAL2GSOCKET(socket)));
}

/* TODO: lookup_type */

/* TODO: register_type */

void
Init_gsocketconnection(VALUE glib)
{
        VALUE socketconnection, socketconnectionfactory;

        socketconnection = G_DEF_CLASS(G_TYPE_SOCKET_CONNECTION, "SocketConnection", glib);

        rb_define_method(socketconnection, "remote_address", socketconnection_get_remote_address, 0);
        rb_define_method(socketconnection, "socket", socketconnection_get_socket, 0);

        /* Perhaps just move this to SocketConnection? */
        socketconnectionfactory = rb_define_module_under(glib, "SocketConnectionFactory");

        rb_define_module_function(socketconnectionfactory, "create_connection", socketconnectionfactory_create_connection, 1);
}
