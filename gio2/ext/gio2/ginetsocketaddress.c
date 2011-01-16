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

static VALUE
inetsocketaddress_initialize(VALUE self, VALUE address, VALUE port)
{
        G_INITIALIZE(self, g_inet_socket_address_new(RVAL2GINETADDRESS(address),
                                                     RVAL2GUINT16(port)));

        return Qnil;
}

void
Init_ginetsocketaddress(VALUE glib)
{
        VALUE inetsocketaddress = G_DEF_CLASS(G_TYPE_INET_SOCKET_ADDRESS, "InetSocketAddress", glib);

        rb_define_method(inetsocketaddress, "initialize", inetsocketaddress_initialize, 2);
}
