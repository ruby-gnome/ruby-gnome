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

#define RG_TARGET_NAMESPACE cSocketClient
#define _SELF(value) RVAL2GSOCKETCLIENT(value)

static VALUE
rg_initialize(VALUE self)
{
        G_INITIALIZE(self, g_socket_client_new());

        return Qnil;
}

static VALUE
rg_connect(int argc, VALUE *argv, VALUE self)
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
rg_connect_async(int argc, VALUE *argv, VALUE self)
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
rg_connect_finish(VALUE self, VALUE result)
{
        return connection_finish_method(g_socket_client_connect_finish, self, result);
}

static VALUE
rg_connect_to_host(int argc, VALUE *argv, VALUE self)
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
rg_connect_to_host_async(int argc, VALUE *argv, VALUE self)
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
rg_connect_to_host_finish(VALUE self, VALUE result)
{
        return connection_finish_method(g_socket_client_connect_to_host_finish, self, result);
}

static VALUE
rg_connect_to_service(int argc, VALUE *argv, VALUE self)
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
rg_connect_to_service_async(int argc, VALUE *argv, VALUE self)
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
rg_connect_to_service_finish(VALUE self, VALUE result)
{
        return connection_finish_method(g_socket_client_connect_to_service_finish, self, result);
}

void
Init_gsocketclient(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_SOCKET_CLIENT, "SocketClient", mGio);

        RG_DEF_METHOD(initialize, 0);
        RG_DEF_METHOD(connect, -1);
        RG_DEF_METHOD(connect_async, -1);
        RG_DEF_METHOD(connect_finish, 1);
        RG_DEF_METHOD(connect_to_host, -1);
        RG_DEF_METHOD(connect_to_host_async, -1);
        RG_DEF_METHOD(connect_to_host_finish, 1);
        RG_DEF_METHOD(connect_to_service, -1);
        RG_DEF_METHOD(connect_to_service_async, -1);
        RG_DEF_METHOD(connect_to_service_finish, 1);
}
