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

#define _SELF(value) G_NETWORK_ADDRESS(RVAL2GOBJ(value))

static VALUE
networkaddress_initialize(VALUE self, VALUE hostname, VALUE port)
{
        G_INITIALIZE(self, g_network_address_new(RVAL2CSTR(hostname),
                                                 RVAL2GUINT16(port)));

        return Qnil;
}

static VALUE
networkaddress_parse(G_GNUC_UNUSED VALUE self, VALUE host_and_port, VALUE default_port)
{
        GError *error = NULL;
        GSocketConnectable *connectable;

        connectable = g_network_address_parse(RVAL2CSTR(host_and_port),
                                              RVAL2GUINT16(default_port),
                                              &error);
        if (connectable == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(connectable);
}

void
Init_gnetworkaddress(VALUE glib)
{
        VALUE networkaddress = G_DEF_CLASS(G_TYPE_NETWORK_ADDRESS, "NetworkAddress", glib);

        rb_define_singleton_method(networkaddress, "parse", networkaddress_parse, 2);

        rb_define_method(networkaddress, "initialize", networkaddress_initialize, 2);
}
