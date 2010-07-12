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

#define _SELF(value) G_THREADED_SOCKET_SERVICE(RVAL2GOBJ(value))

static VALUE
threadedsocketservice_initialize(VALUE self)
{
        G_INITIALIZE(self, GOBJ2RVAL(g_socket_service_new()));

        return Qnil;
}

void
Init_gthreadedsocketservice(VALUE glib)
{
        VALUE threadedsocketservice = G_DEF_CLASS(G_TYPE_THREADED_SOCKET_SERVICE, "ThreadedSocketService", glib);

        rb_define_method(threadedsocketservice, "initialize", threadedsocketservice_initialize, 0);
}
