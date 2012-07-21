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

#define RG_TARGET_NAMESPACE cSocket
#define _SELF(value) RVAL2GSOCKET(value)

static ID id_call;

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE family, type, protocol;
        GError *error = NULL;
        GSocket *socket;

        rb_scan_args(argc, argv, "21", &family, &type, &protocol);
        socket = g_socket_new(RVAL2GSOCKETFAMILY(family),
                              RVAL2GSOCKETTYPE(type),
                              RVAL2GSOCKETPROTOCOLDEFAULT(protocol),
                              &error);
        if (socket == NULL)
                rbgio_raise_error(error);

        G_INITIALIZE(self, socket);

        return Qnil;
}

static VALUE
rg_s_new_from_fd(G_GNUC_UNUSED VALUE self, VALUE id)
{
        GError *error = NULL;
        GSocket *socket;

        socket = g_socket_new_from_fd(RVAL2FD(id), &error);
        if (socket == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(socket);
}

static VALUE
rg_bind(VALUE self, VALUE address, VALUE allow_reuse)
{
        GError *error = NULL;

        if (!g_socket_bind(_SELF(self),
                           RVAL2GSOCKETADDRESS(address),
                           RVAL2CBOOL(allow_reuse),
                           &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_listen(VALUE self)
{
        GError *error = NULL;

        if (!g_socket_listen(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_accept(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        GSocket *socket;

        rb_scan_args(argc, argv, "01", &cancellable);
        socket = g_socket_accept(_SELF(self),
                                 RVAL2GCANCELLABLE(cancellable),
                                 &error);
        if (socket == NULL)
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_connect(int argc, VALUE *argv, VALUE self)
{
        VALUE address, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &address, &cancellable);
        if (!g_socket_connect(_SELF(self),
                              RVAL2GSOCKETADDRESS(address),
                              RVAL2GCANCELLABLE(cancellable),
                              &error))
                rbgio_raise_error(error);

        return self;
}

/* TODO: This makes very little sense in Ruby.  How do we deal with it? */
static VALUE
rg_check_connect_result(VALUE self)
{
        GError *error = NULL;

        if (!g_socket_check_connect_result(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_receive(int argc, VALUE *argv, VALUE self)
{
        VALUE rbbytes, cancellable, result;
        GError *error = NULL;
        gsize bytes;
        gssize read;

        rb_scan_args(argc, argv, "11", &rbbytes, &cancellable);
        bytes = RVAL2GSIZE(rbbytes);
        result = rb_str_new(NULL, bytes);
        read = g_socket_receive(_SELF(self),
                                RSTRING_PTR(result),
                                bytes,
                                RVAL2GCANCELLABLE(cancellable),
                                &error);
        if (read == -1)
                rbgio_raise_error(error);

        /* TODO: This is what Ruby does, but I can't figure out why one needs
         * both. */
        rb_str_set_len(result, read);
        rb_str_resize(result, read);
        OBJ_TAINT(result);

        return result;
}

static VALUE
rg_receive_from(int argc, VALUE *argv, VALUE self)
{
        VALUE rbbytes, cancellable, result;
        GSocketAddress *address;
        GError *error = NULL;
        gsize bytes;
        gssize read;

        rb_scan_args(argc, argv, "11", &rbbytes, &cancellable);
        bytes = RVAL2GSIZE(rbbytes);
        result = rb_str_new(NULL, bytes);
        read = g_socket_receive_from(_SELF(self),
                                     &address,
                                     RSTRING_PTR(result),
                                     bytes,
                                     RVAL2GCANCELLABLE(cancellable),
                                     &error);
        if (read == -1)
                rbgio_raise_error(error);

        rb_str_set_len(result, read);
        rb_str_resize(result, read);
        OBJ_TAINT(result);

        return rb_assoc_new(GOBJ2RVAL_UNREF(address), result);
}

static VALUE
rg_send(int argc, VALUE *argv, VALUE self)
{
        VALUE buffer, cancellable;
        GError *error = NULL;
        gssize sent;

        rb_scan_args(argc, argv, "11", &buffer, &cancellable);
        StringValue(buffer);
        sent = g_socket_send(_SELF(self),
                             RSTRING_PTR(buffer),
                             RSTRING_LEN(buffer),
                             RVAL2GCANCELLABLE(cancellable),
                             &error);
        if (sent == -1)
                rbgio_raise_error(error);

        return GSSIZE2RVAL(sent);
}

static VALUE
rg_send_to(int argc, VALUE *argv, VALUE self)
{
        VALUE address, buffer, cancellable;
        GError *error = NULL;
        gssize sent;

        rb_scan_args(argc, argv, "11", &address, &buffer, &cancellable);
        StringValue(buffer);
        sent = g_socket_send_to(_SELF(self),
                                RVAL2GSOCKETADDRESS(address),
                                RSTRING_PTR(buffer),
                                RSTRING_LEN(buffer),
                                RVAL2GCANCELLABLE(cancellable),
                                &error);
        if (sent == -1)
                rbgio_raise_error(error);

        return GSSIZE2RVAL(sent);
}

static VALUE
rg_close(VALUE self)
{
        GError *error = NULL;

        if (!g_socket_close(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_closed_p(VALUE self)
{
        return CBOOL2RVAL(g_socket_is_closed(_SELF(self)));
}

static VALUE
rg_shutdown(VALUE self, VALUE shutdown_read, VALUE shutdown_write)
{
        GError *error = NULL;

        if (!g_socket_shutdown(_SELF(self),
                               RVAL2CBOOL(shutdown_read),
                               RVAL2CBOOL(shutdown_write),
                               &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_connected_p(VALUE self)
{
        return (g_socket_is_connected(_SELF(self)));
}

static gboolean
source_func(GSocket *socket, GIOCondition condition, gpointer user_data)
{
        VALUE callback = GPOINTER_TO_UINT(user_data);
        return RVAL2CBOOL(rb_funcall(callback, id_call, 2,
                                     GOBJ2RVAL(socket),
                                     GIOCONDITION2RVAL(condition)));
}

static VALUE
rg_create_source(int argc, VALUE *argv, VALUE self)
{
        VALUE condition, cancellable, callback;
        GSource *source;

        rb_scan_args(argc, argv, "11&", &condition, &cancellable, &callback);

        source = g_socket_create_source(_SELF(self),
                                        RVAL2GIOCONDITION(condition),
                                        RVAL2GCANCELLABLE(cancellable));
        G_RELATIVE(self, callback);
        g_source_set_callback(source,
                              (GSourceFunc)source_func,
                              GUINT_TO_POINTER(callback),
                              NULL);
        return GSOURCE2RVAL(source);
}

static VALUE
rg_condition_check(VALUE self, VALUE condition)
{
        return GIOCONDITION2RVAL(g_socket_condition_check(_SELF(self),
                                                          RVAL2GIOCONDITION(condition)));
}

static VALUE
rg_condition_wait(int argc, VALUE *argv, VALUE self)
{
        VALUE condition, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &condition, &cancellable);
        if (!g_socket_condition_wait(_SELF(self),
                                     RVAL2GIOCONDITION(condition),
                                     RVAL2GCANCELLABLE(cancellable),
                                     &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
socket_get_local_address(VALUE self)
{
        GError *error = NULL;
        GSocketAddress *address;

        address = g_socket_get_local_address(_SELF(self), &error);
        if (address == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(address);
}

static VALUE
socket_get_remote_address(VALUE self)
{
        GError *error = NULL;
        GSocketAddress *address;

        address = g_socket_get_remote_address(_SELF(self), &error);
        if (address == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(address);
}

static VALUE
rg_speaks_ipv4_p(VALUE self)
{
        return CBOOL2RVAL(g_socket_speaks_ipv4(_SELF(self)));
}

void
Init_gsocket(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_SOCKET, "Socket", mGio);

        id_call = rb_intern("call");

        G_DEF_CLASS(G_TYPE_SOCKET_FAMILY, "Family", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_SOCKET_FAMILY, "G_SOCKET_");

        G_DEF_CLASS(G_TYPE_SOCKET_TYPE, "Type", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_SOCKET_TYPE, "G_SOCKET_");

        G_DEF_CLASS(G_TYPE_SOCKET_PROTOCOL, "Protocol", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_SOCKET_PROTOCOL, "G_SOCKET_");

        G_DEF_CLASS(G_TYPE_SOCKET_MSG_FLAGS, "MsgFlags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_SOCKET_MSG_FLAGS, "G_SOCKET_");

        RG_DEF_SMETHOD(new_from_fd, 1);

        RG_DEF_METHOD(initialize, -1);
        RG_DEF_METHOD(bind, 2);
        RG_DEF_METHOD(listen, 0);
        RG_DEF_METHOD(accept, -1);
        RG_DEF_METHOD(connect, -1);
        RG_DEF_METHOD(check_connect_result, 0);
        RG_DEF_METHOD(receive, -1);
        RG_DEF_METHOD(receive_from, -1);
        RG_DEF_METHOD(send, -1);
        RG_DEF_METHOD(send_to, -1);
        RG_DEF_METHOD(close, 0);
        RG_DEF_METHOD_P(closed, 0);
        RG_DEF_METHOD(shutdown, 2);
        RG_DEF_METHOD_P(connected, 0);
        RG_DEF_METHOD(create_source, -1);
        RG_DEF_METHOD(condition_check, 1);
        RG_DEF_METHOD(condition_wait, -1);
        G_REPLACE_GET_PROPERTY(RG_TARGET_NAMESPACE, "local_address", socket_get_local_address, 0);
        G_REPLACE_GET_PROPERTY(RG_TARGET_NAMESPACE, "remote_address", socket_get_remote_address, 0);
        RG_DEF_METHOD_P(speaks_ipv4, 0);
}
