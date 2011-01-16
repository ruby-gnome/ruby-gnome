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

#define _SELF(value) G_NETWORK_SERVICE(RVAL2GOBJ(value))

static VALUE
networkservice_initialize(VALUE self, VALUE service, VALUE protocol, VALUE domain)
{
        /* TODO: If these are infected, so should they be when returned.  How
         * do we deal with that? */
        G_INITIALIZE(self, g_network_service_new(RVAL2CSTR(service),
                                                 RVAL2CSTR(protocol),
                                                 RVAL2CSTR(domain)));

        return Qnil;
}

void
Init_gnetworkservice(VALUE glib)
{
        VALUE networkservice = G_DEF_CLASS(G_TYPE_NETWORK_SERVICE, "NetworkService", glib);

        rb_define_method(networkservice, "initialize", networkservice_initialize, 3);
}
