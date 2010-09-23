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

#define _SELF(value) G_SOCKET_SERVICE(RVAL2GOBJ(value))

static VALUE
socketservice_initialize(VALUE self)
{
        G_INITIALIZE(self, g_socket_service_new());

        return Qnil;
}

static VALUE
socketservice_start(VALUE self)
{
        g_socket_service_start(_SELF(self));

        return self;
}

static VALUE
socketservice_stop(VALUE self)
{
        g_socket_service_stop(_SELF(self));

        return self;
}

static VALUE
socketservice_is_active(VALUE self)
{
        return CBOOL2RVAL(g_socket_service_is_active(_SELF(self)));
}

void
Init_gsocketservice(VALUE glib)
{
        VALUE socketservice = G_DEF_CLASS(G_TYPE_SOCKET_SERVICE, "SocketService", glib);

        rb_define_method(socketservice, "initialize", socketservice_initialize, 0);
        rb_define_method(socketservice, "start", socketservice_start, 0);
        rb_define_method(socketservice, "stop", socketservice_stop, 0);
        rb_define_method(socketservice, "active?", socketservice_is_active, 0);
}
