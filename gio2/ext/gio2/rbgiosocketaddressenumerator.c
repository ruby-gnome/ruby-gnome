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

#define RG_TARGET_NAMESPACE cSocketAddressEnumerator
#define _SELF(value) G_SOCKET_ADDRESS_ENUMERATOR(RVAL2GOBJ(value))

static VALUE
rg_next(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        GSocketAddress *address;

        rb_scan_args(argc, argv, "01", &cancellable);
        address = g_socket_address_enumerator_next(_SELF(self),
                                                   RVAL2GCANCELLABLE(cancellable),
                                                   &error);
        if (error != NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL(address);
}

static VALUE
rg_next_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcancellable, block;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "01&", &rbcancellable, &block);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_socket_address_enumerator_next_async(_SELF(self),
                                               cancellable,
                                               rbgio_async_ready_callback,
                                               (gpointer)block);

        return self;
}

static VALUE
rg_next_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GSocketAddress *address;

        address = g_socket_address_enumerator_next_finish(_SELF(self),
                                                          RVAL2GASYNCRESULT(result),
                                                          &error);
        if (address == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL(address);
}

void
Init_gsocketaddressenumerator(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_SOCKET_ADDRESS_ENUMERATOR, "SocketAddressEnumerator", mGio);

        RG_DEF_METHOD(next, -1);
        RG_DEF_METHOD(next_async, -1);
        RG_DEF_METHOD(next_finish, 1);
}
