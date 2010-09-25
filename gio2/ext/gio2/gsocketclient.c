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

#define _SELF(value) G_SOCKET_CLIENT(RVAL2GOBJ(value))

static VALUE
socketclient_initialize(VALUE self)
{
        G_INITIALIZE(self, g_socket_client_new());

        return Qnil;
}

static VALUE
socketclient_connect(int argc, VALUE *argv, VALUE self)
{
        VALUE connectable, cancellable;
        GError *error = NULL;
        GSocketConnection *connection;

        rb_scan_args(argc, argv, "11", &connectable, &cancellable);
        connection = g_socket_client_connect(_SELF(self),
                                             RVAL2GSOCKETCONNECTABLE(connectable),
                                             RVAL2GCANCELLABLE(cancellable),
                                             &error);
        if (connection == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(connection);
}

static VALUE
socketclient_connect_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbconnectable, rbcancellable, block;
        GCancellable *cancellable;
        GSocketConnectable *connectable;

        rb_scan_args(argc, argv, "11&", &rbconnectable, &rbcancellable, &block);
        connectable = RVAL2GSOCKETCONNECTABLE(rbconnectable);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_socket_client_connect_async(_SELF(self),
                                      connectable,
                                      cancellable,
                                      rbgio_async_ready_callback,
                                      (gpointer)block);

        return self;
}

typedef GSocketConnection *(*ConnectionFinishMethod)(GSocketClient *,
                                                     GAsyncResult *,
                                                     GError **);

static VALUE
connection_finish_method(ConnectionFinishMethod method, VALUE self, VALUE result)
{
        GError *error = NULL;
        GSocketConnection *connection;

        connection = method(_SELF(self), RVAL2GASYNCRESULT(result), &error);
        if (connection == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(connection);
}

static VALUE
socketclient_connect_finish(VALUE self, VALUE result)
{
        return connection_finish_method(g_socket_client_connect_finish, self, result);
}

static VALUE
socketclient_connect_to_host(int argc, VALUE *argv, VALUE self)
{
        VALUE host_and_port, default_port, cancellable;
        GError *error = NULL;
        GSocketConnection *connection;

        rb_scan_args(argc, argv, "21", &host_and_port, &default_port, &cancellable);
        connection = g_socket_client_connect_to_host(_SELF(self),
                                                     RVAL2CSTR(host_and_port),
                                                     RVAL2GUINT16(default_port),
                                                     RVAL2GCANCELLABLE(cancellable),
                                                     &error);
        if (connection == NULL)
                rbgio_raise_error(error);

        return self;
}

static VALUE
socketclient_connect_to_host_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbhost_and_port, rbdefault_port, rbcancellable, block;
        GCancellable *cancellable;
        const gchar *host_and_port;
        guint16 default_port;

        rb_scan_args(argc, argv, "21&", &rbhost_and_port, &rbdefault_port, &rbcancellable, &block);
        host_and_port = RVAL2CSTR(rbhost_and_port);
        default_port = RVAL2GUINT16(rbdefault_port);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_socket_client_connect_to_host_async(_SELF(self),
                                              host_and_port,
                                              default_port,
                                              cancellable,
                                              rbgio_async_ready_callback,
                                              (gpointer)block);

        return self;
}

static VALUE
socketclient_connect_to_host_finish(VALUE self, VALUE result)
{
        return connection_finish_method(g_socket_client_connect_to_host_finish, self, result);
}

static VALUE
socketclient_connect_to_service(int argc, VALUE *argv, VALUE self)
{
        VALUE domain, service, cancellable;
        GError *error = NULL;
        GSocketConnection *connection;

        rb_scan_args(argc, argv, "21", &domain, &service, &cancellable);
        connection = g_socket_client_connect_to_service(_SELF(self),
                                                        RVAL2CSTR(domain),
                                                        RVAL2CSTR(service),
                                                        RVAL2GCANCELLABLE(cancellable),
                                                        &error);
        if (connection == NULL)
                rbgio_raise_error(error);

        return self;
}

static VALUE
socketclient_connect_to_service_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbdomain, rbservice, rbcancellable, block;
        GCancellable *cancellable;
        const gchar *domain;
        const gchar *service;

        rb_scan_args(argc, argv, "21&", &domain, &service, &rbcancellable, &block);
        domain = RVAL2CSTR(rbdomain);
        service = RVAL2CSTR(rbservice);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_socket_client_connect_to_service_async(_SELF(self),
                                                 domain,
                                                 service,
                                                 cancellable,
                                                 rbgio_async_ready_callback,
                                                 (gpointer)block);

        return self;
}

static VALUE
socketclient_connect_to_service_finish(VALUE self, VALUE result)
{
        return connection_finish_method(g_socket_client_connect_to_service_finish, self, result);
}

static VALUE
socketclient_set_family(VALUE self, VALUE family)
{
        g_socket_client_set_family(_SELF(self), RVAL2GSOCKETFAMILY(family));

        return self;
}

static VALUE
socketclient_set_local_address(VALUE self, VALUE address)
{
        g_socket_client_set_local_address(_SELF(self), RVAL2GSOCKETADDRESS(address));

        return self;
}

static VALUE
socketclient_set_protocol(VALUE self, VALUE protocol)
{
        g_socket_client_set_protocol(_SELF(self), RVAL2GSOCKETPROTOCOL(protocol));

        return self;
}

static VALUE
socketclient_set_socket_type(VALUE self, VALUE type)
{
        g_socket_client_set_socket_type(_SELF(self), RVAL2GSOCKETTYPE(type));

        return self;
}

static VALUE
socketclient_get_family(VALUE self)
{
        return GSOCKETFAMILY2RVAL(g_socket_client_get_family(_SELF(self)));
}

static VALUE
socketclient_get_local_address(VALUE self)
{
        return GOBJ2RVAL(g_socket_client_get_local_address(_SELF(self)));
}

static VALUE
socketclient_get_protocol(VALUE self)
{
        return GSOCKETPROTOCOL2RVAL(g_socket_client_get_protocol(_SELF(self)));
}

static VALUE
socketclient_get_socket_type(VALUE self)
{
        return GSOCKETTYPE2RVAL(g_socket_client_get_socket_type(_SELF(self)));
}

void
Init_gsocketclient(VALUE glib)
{
        VALUE socketclient = G_DEF_CLASS(G_TYPE_SOCKET_CLIENT, "SocketClient", glib);

        rb_define_method(socketclient, "initialize", socketclient_initialize, 0);
        rb_define_method(socketclient, "connect", socketclient_connect, -1);
        rb_define_method(socketclient, "connect_async", socketclient_connect_async, -1);
        rb_define_method(socketclient, "connect_finish", socketclient_connect_finish, 1);
        rb_define_method(socketclient, "connect_to_host", socketclient_connect_to_host, -1);
        rb_define_method(socketclient, "connect_to_host_async", socketclient_connect_to_host_async, -1);
        rb_define_method(socketclient, "connect_to_host_finish", socketclient_connect_to_host_finish, 1);
        rb_define_method(socketclient, "connect_to_service", socketclient_connect_to_service, -1);
        rb_define_method(socketclient, "connect_to_service_async", socketclient_connect_to_service_async, -1);
        rb_define_method(socketclient, "connect_to_service_finish", socketclient_connect_to_service_finish, 1);
        rb_define_method(socketclient, "set_family", socketclient_set_family, 1);
        G_DEF_SETTER(socketclient, "family");
        rb_define_method(socketclient, "set_local_address", socketclient_set_local_address, 1);
        G_DEF_SETTER(socketclient, "local_address");
        rb_define_method(socketclient, "set_protocol", socketclient_set_protocol, 1);
        G_DEF_SETTER(socketclient, "protocol");
        rb_define_method(socketclient, "set_socket_type", socketclient_set_socket_type, 1);
        G_DEF_SETTER(socketclient, "socket_type");
        rb_define_method(socketclient, "family", socketclient_get_family, 0);
        rb_define_method(socketclient, "local_address", socketclient_get_local_address, 0);
        rb_define_method(socketclient, "protocol", socketclient_get_protocol, 0);
        rb_define_method(socketclient, "socket_type", socketclient_get_socket_type, 0);
}
