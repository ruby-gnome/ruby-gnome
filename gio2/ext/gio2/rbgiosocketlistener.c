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

#define RG_TARGET_NAMESPACE cSocketListener
#define _SELF(value) RVAL2GSOCKETLISTENER(value)

static VALUE
rg_initialize(VALUE self)
{
        G_INITIALIZE(self, g_socket_listener_new());

        return Qnil;
}

static VALUE
rg_add_socket(int argc, VALUE *argv, VALUE self)
{
        VALUE socket, source_object;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &socket, &source_object);
        if (!g_socket_listener_add_socket(_SELF(self), RVAL2GSOCKET(socket),
                                          RVAL2GOBJ(source_object), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_add_address(int argc, VALUE *argv, VALUE self)
{
        VALUE rbaddress, type, protocol, source_object;
        GError *error = NULL;
        GSocketAddress *address;

        rb_scan_args(argc, argv, "22", &rbaddress, &type, &protocol, &source_object);
        if (!g_socket_listener_add_address(_SELF(self),
                                           RVAL2GSOCKETADDRESS(rbaddress),
                                           RVAL2GSOCKETTYPE(type),
                                           RVAL2GSOCKETPROTOCOLDEFAULT(protocol),
                                           RVAL2GOBJ(source_object),
                                           &address,
                                           &error))
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(address);
}

static VALUE
rg_add_inet_port(int argc, VALUE *argv, VALUE self)
{
        VALUE port, source_object;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &port, &source_object);
        if (!g_socket_listener_add_inet_port(_SELF(self),
                                             RVAL2GUINT16(port),
                                             RVAL2GOBJ(source_object),
                                             &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_add_any_inet_port(int argc, VALUE *argv, VALUE self)
{
        VALUE source_object;
        GError *error = NULL;
        guint16 port;

        rb_scan_args(argc, argv, "01", &source_object);
        port = g_socket_listener_add_any_inet_port(_SELF(self),
                                                   RVAL2GOBJ(source_object),
                                                   &error);
        if (port == 0)
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_accept(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GObject *source_object;
        GError *error = NULL;
        GSocketConnection *connection;

        rb_scan_args(argc, argv, "01", &cancellable);
        connection = g_socket_listener_accept(_SELF(self),
                                              &source_object,
                                              RVAL2GCANCELLABLE(cancellable),
                                              &error);
        if (connection == NULL)
                rbgio_raise_error(error);

        return rb_assoc_new(GOBJ2RVAL_UNREF(connection),
                            GOBJ2RVAL(source_object));
}

static VALUE
rg_accept_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcancellable, block;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "01&", &rbcancellable, &block);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_socket_listener_accept_async(_SELF(self),
                                       cancellable,
                                       rbgio_async_ready_callback,
                                       (gpointer)block);

        return self;
}

static VALUE
rg_accept_finish(VALUE self, VALUE result)
{
        GObject *source_object;
        GError *error = NULL;
        GSocketConnection *connection;

        connection = g_socket_listener_accept_finish(_SELF(self),
                                                     RVAL2GASYNCRESULT(result),
                                                     &source_object,
                                                     &error);
        if (connection == NULL)
                rbgio_raise_error(error);

        return rb_assoc_new(GOBJ2RVAL_UNREF(connection),
                            GOBJ2RVAL(source_object));
}

static VALUE
rg_accept_socket(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GObject *source_object;
        GError *error = NULL;
        GSocket *socket;

        rb_scan_args(argc, argv, "01", &cancellable);
        socket = g_socket_listener_accept_socket(_SELF(self),
                                                 &source_object,
                                                 RVAL2GCANCELLABLE(cancellable),
                                                 &error);
        if (socket == NULL)
                rbgio_raise_error(error);

        return rb_assoc_new(GOBJ2RVAL_UNREF(socket),
                            GOBJ2RVAL(source_object));
}

static VALUE
rg_accept_socket_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcancellable, block;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "01&", &rbcancellable, &block);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_socket_listener_accept_socket_async(_SELF(self),
                                              cancellable,
                                              rbgio_async_ready_callback,
                                              (gpointer)block);

        return self;
}

static VALUE
rg_accept_socket_finish(VALUE self, VALUE result)
{
        GObject *source_object;
        GError *error = NULL;
        GSocket *socket;

        socket = g_socket_listener_accept_socket_finish(_SELF(self),
                                                        RVAL2GASYNCRESULT(result),
                                                        &source_object,
                                                        &error);
        if (socket == NULL)
                rbgio_raise_error(error);

        return rb_assoc_new(GOBJ2RVAL_UNREF(socket),
                            GOBJ2RVAL(source_object));
}

static VALUE
rg_close(VALUE self)
{
        g_socket_listener_close(_SELF(self));

        return self;
}

static VALUE
rg_set_backlog(VALUE self, VALUE listen_backlog)
{
        g_socket_listener_set_backlog(_SELF(self), FIX2INT(listen_backlog));

        return self;
}

void
Init_gsocketlistener(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_SOCKET_LISTENER, "SocketListener", mGio);

        RG_DEF_METHOD(initialize, 0);
        RG_DEF_METHOD(add_socket, -1);
        RG_DEF_METHOD(add_address, -1);
        RG_DEF_METHOD(add_inet_port, -1);
        RG_DEF_METHOD(add_any_inet_port, -1);
        RG_DEF_METHOD(accept, -1);
        RG_DEF_METHOD(accept_async, -1);
        RG_DEF_METHOD(accept_finish, 1);
        RG_DEF_METHOD(accept_socket, -1);
        RG_DEF_METHOD(accept_socket_async, -1);
        RG_DEF_METHOD(accept_socket_finish, 1);
        RG_DEF_METHOD(close, 0);
        RG_DEF_METHOD(set_backlog, 1);
}
