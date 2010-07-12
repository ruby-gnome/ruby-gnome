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

#define _SELF(value) G_TCP_CONNECTION(RVAL2GOBJ(value))

static VALUE
tcpconnection_set_graceful_disconnect(VALUE self, VALUE graceful_disconnect)
{
        g_tcp_connection_set_graceful_disconnect(_SELF(self),
                                                 RVAL2CBOOL(graceful_disconnect));

        return self;
}

static VALUE
tcpconnection_get_graceful_disconnect(VALUE self)
{
        return CBOOL2RVAL(g_tcp_connection_get_graceful_disconnect(_SELF(self)));
}

void
Init_gtcpconnection(VALUE glib)
{
        VALUE tcpconnection = G_DEF_CLASS(G_TYPE_TCP_CONNECTION, "TcpConnection", glib);

        rb_define_method(tcpconnection, "set_graceful_disconnect", tcpconnection_set_graceful_disconnect, 1);
        G_DEF_SETTER(tcpconnection, "graceful_disconnect");
        rb_define_method(tcpconnection, "graceful_disconnect?", tcpconnection_get_graceful_disconnect, 0);
}
