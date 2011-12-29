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

#define RG_TARGET_NAMESPACE cSocketService
#define _SELF(value) RVAL2GSOCKETSERVICE(value)

static VALUE
rg_initialize(VALUE self)
{
        G_INITIALIZE(self, g_socket_service_new());

        return Qnil;
}

static VALUE
rg_start(VALUE self)
{
        g_socket_service_start(_SELF(self));

        return self;
}

static VALUE
rg_stop(VALUE self)
{
        g_socket_service_stop(_SELF(self));

        return self;
}

static VALUE
rg_active_p(VALUE self)
{
        return CBOOL2RVAL(g_socket_service_is_active(_SELF(self)));
}

void
Init_gsocketservice(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_SOCKET_SERVICE, "SocketService", mGio);

        RG_DEF_METHOD(initialize, 0);
        RG_DEF_METHOD(start, 0);
        RG_DEF_METHOD(stop, 0);
        RG_DEF_METHOD_P(active, 0);
}
