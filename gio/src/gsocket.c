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

#define _SELF(value) RVAL2GSOCKET(value)

#define RVAL2GIOCONDITION(value) RVAL2GFLAGS((value), G_TYPE_IO_CONDITION)

#define GIOCONDITION2RVAL(value) GENUM2RVAL((value), G_TYPE_IO_CONDITION)

static VALUE
socket_initialize(int argc, VALUE *argv, VALUE self)
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

        G_INITIALIZE(self, GOBJ2RVAL(socket));

        return Qnil;
}

static VALUE
socket_new_from_fd(G_GNUC_UNUSED VALUE self, VALUE id)
{
        GError *error = NULL;
        GSocket *socket;

        socket = g_socket_new_from_fd(RVAL2FD(id), &error);
        if (socket == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL(socket);
}

static VALUE
socket_bind(VALUE self, VALUE address, VALUE allow_reuse)
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
socket_listen(VALUE self)
{
        GError *error = NULL;

        if (!g_socket_listen(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
socket_accept(int argc, VALUE *argv, VALUE self)
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
socket_connect(int argc, VALUE *argv, VALUE self)
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
socket_check_connect_result(VALUE self)
{
        GError *error = NULL;

        if (!g_socket_check_connect_result(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
socket_receive(int argc, VALUE *argv, VALUE self)
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

        /* TODO: This is what Ruby does, but I can’t figure out why one needs
         * both. */
        rb_str_set_len(result, read);
        rb_str_resize(result, read);
        OBJ_TAINT(result);

        return result;
}

static VALUE
socket_receive_from(int argc, VALUE *argv, VALUE self)
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

        return rb_assoc_new(GOBJ2RVAL(address), result);
}

static VALUE
socket_send(int argc, VALUE *argv, VALUE self)
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
socket_send_to(int argc, VALUE *argv, VALUE self)
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
socket_close(VALUE self)
{
        GError *error = NULL;

        if (!g_socket_close(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
socket_is_closed(VALUE self)
{
        return CBOOL2RVAL(g_socket_is_closed(_SELF(self)));
}

static VALUE
socket_shutdown(VALUE self, VALUE shutdown_read, VALUE shutdown_write)
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
socket_is_connected(VALUE self)
{
        return (g_socket_is_connected(_SELF(self)));
}

static VALUE
socket_create_source(int argc, VALUE *argv, VALUE self)
{
        VALUE condition, cancellable;

        rb_scan_args(argc, argv, "11", &condition, &cancellable);

        return GOBJ2RVAL(g_socket_create_source(_SELF(self),
                                                RVAL2GIOCONDITION(condition),
                                                RVAL2GCANCELLABLE(cancellable)));
}

static VALUE
socket_condition_check(VALUE self, VALUE condition)
{
        return GIOCONDITION2RVAL(g_socket_condition_check(_SELF(self),
                                                          RVAL2GIOCONDITION(condition)));
}

static VALUE
socket_condition_wait(int argc, VALUE *argv, VALUE self)
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
socket_set_listen_backlog(VALUE self, VALUE backlog)
{
        g_socket_set_listen_backlog(_SELF(self), RVAL2GINT(backlog));

        return self;
}

static VALUE
socket_get_listen_backlog(VALUE self)
{
        return GINT2RVAL(g_socket_get_listen_backlog(_SELF(self)));
}

static VALUE
socket_get_blocking(VALUE self)
{
        return CBOOL2RVAL(g_socket_get_blocking(_SELF(self)));
}

static VALUE
socket_set_blocking(VALUE self, VALUE blocking)
{
        g_socket_set_blocking(_SELF(self), RVAL2CBOOL(blocking));

        return self;
}

static VALUE
socket_get_keepalive(VALUE self)
{
        return CBOOL2RVAL(g_socket_get_keepalive(_SELF(self)));
}

static VALUE
socket_set_keepalive(VALUE self, VALUE keepalive)
{
        g_socket_set_keepalive(_SELF(self), RVAL2CBOOL(keepalive));

        return self;
}

static VALUE
socket_get_family(VALUE self)
{
        return GSOCKETFAMILY2RVAL(g_socket_get_family(_SELF(self)));
}

static VALUE
socket_get_fd(VALUE self)
{
        return FD2RVAL(g_socket_get_fd(_SELF(self)));
}

static VALUE
socket_get_local_address(VALUE self)
{
        GError *error = NULL;
        GSocketAddress *address;

        address = g_socket_get_local_address(_SELF(self), &error);
        if (address == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL(address);
}

static VALUE
socket_get_protocol(VALUE self)
{
        return GSOCKETPROTOCOL2RVAL(g_socket_get_protocol(_SELF(self)));
}

static VALUE
socket_get_remote_address(VALUE self)
{
        GError *error = NULL;
        GSocketAddress *address;

        address = g_socket_get_remote_address(_SELF(self), &error);
        if (address == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL(address);
}

static VALUE
socket_get_socket_type(VALUE self)
{
        return GSOCKETTYPE2RVAL(g_socket_get_socket_type(_SELF(self)));
}

static VALUE
socket_speaks_ipv4(VALUE self)
{
        return CBOOL2RVAL(g_socket_speaks_ipv4(_SELF(self)));
}

void
Init_gsocket(VALUE glib)
{
        VALUE socket = G_DEF_CLASS(G_TYPE_SOCKET_ADDRESS, "Socket", glib);

        G_DEF_CLASS(G_TYPE_SOCKET_FAMILY, "Family", socket);
        G_DEF_CONSTANTS(socket, G_TYPE_SOCKET_FAMILY, "G_SOCKET_");

        G_DEF_CLASS(G_TYPE_SOCKET_TYPE, "Type", socket);
        G_DEF_CONSTANTS(socket, G_TYPE_SOCKET_TYPE, "G_SOCKET_");

        G_DEF_CLASS(G_TYPE_SOCKET_PROTOCOL, "Protocol", socket);
        G_DEF_CONSTANTS(socket, G_TYPE_SOCKET_PROTOCOL, "G_SOCKET_");

        G_DEF_CLASS(G_TYPE_SOCKET_MSG_FLAGS, "MsgFlags", socket);
        G_DEF_CONSTANTS(socket, G_TYPE_SOCKET_MSG_FLAGS, "G_SOCKET_");

        rb_define_singleton_method(socket, "new_from_fd", socket_new_from_fd, 1);

        rb_define_method(socket, "initialize", socket_initialize, -1);
        rb_define_method(socket, "bind", socket_bind, 2);
        rb_define_method(socket, "listen", socket_listen, 0);
        rb_define_method(socket, "accept", socket_accept, -1);
        rb_define_method(socket, "connect", socket_connect, -1);
        rb_define_method(socket, "check_connect_result", socket_check_connect_result, 0);
        rb_define_method(socket, "receive", socket_receive, -1);
        rb_define_method(socket, "receive_from", socket_receive_from, -1);
        rb_define_method(socket, "send", socket_send, -1);
        rb_define_method(socket, "send_to", socket_send_to, -1);
        rb_define_method(socket, "close", socket_close, -1);
        rb_define_method(socket, "closed?", socket_is_closed, 0);
        rb_define_method(socket, "shutdown", socket_shutdown, 2);
        rb_define_method(socket, "connected?", socket_is_connected, 0);
        rb_define_method(socket, "create_source", socket_create_source, -1);
        rb_define_method(socket, "condition_check", socket_condition_check, 1);
        rb_define_method(socket, "condition_wait", socket_condition_wait, -1);
        rb_define_method(socket, "set_listen_backlog", socket_set_listen_backlog, 1);
        G_DEF_SETTER(socket, "listen_backlog");
        rb_define_method(socket, "listen_backlog", socket_get_listen_backlog, 0);
        rb_define_method(socket, "blocking?", socket_get_blocking, 0);
        rb_define_method(socket, "set_blocking", socket_set_blocking, 1);
        G_DEF_SETTER(socket, "blocking");
        rb_define_method(socket, "keepalive?", socket_get_keepalive, 0);
        rb_define_method(socket, "set_keepalive", socket_set_keepalive, 1);
        G_DEF_SETTER(socket, "keepalive");
        rb_define_method(socket, "family", socket_get_family, 0);
        rb_define_method(socket, "fd", socket_get_fd, 0);
        rb_define_method(socket, "local_address", socket_get_local_address, 0);
        rb_define_method(socket, "protocol", socket_get_protocol, 0);
        rb_define_method(socket, "remote_address", socket_get_remote_address, 0);
        rb_define_method(socket, "socket_type", socket_get_socket_type, 0);
        rb_define_method(socket, "speaks_ipv4?", socket_speaks_ipv4, 0);
}
