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

#define RG_TARGET_NAMESPACE mInetAddress
#define _SELF(value) RVAL2GINETADDRESS(value)

static VALUE
rg_s_new_from_string(G_GNUC_UNUSED VALUE self, VALUE string)
{
        return GOBJ2RVAL_UNREF(g_inet_address_new_from_string(RVAL2CSTR(string)));
}

/* TODO: Should this take an array instead? */
static VALUE
rg_s_new_from_bytes(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
        VALUE rbbytes, rbfamily;
        guint8 *bytes;
        GSocketFamily family;

        rb_scan_args(argc, argv, "11", &rbbytes, &rbfamily);
        bytes = (guint8 *)RVAL2CSTR(rbbytes);
        family = RVAL2GSOCKETFAMILY(rbfamily);

        /* TODO: Should check encoding. */
        /* TODO: We can't pass bytes as such, as it will be NULL-terminated. */
        /* TODO: Do we need to deal with G_SOCKET_FAMILY_UNIX? */
        switch (family) {
        case G_SOCKET_FAMILY_IPV4:
                if (RSTRING_LEN(rbbytes) != 4)
                        rb_raise(rb_eArgError, "string containing 4 bytes expected");
                break;
        case G_SOCKET_FAMILY_IPV6:
                if (RSTRING_LEN(rbbytes) != 16)
                        rb_raise(rb_eArgError, "string containing 16 bytes expected");
                break;
        case G_SOCKET_FAMILY_INVALID:
        case G_SOCKET_FAMILY_UNIX:
                rb_raise(rb_eArgError, "invalid family: %d", family);
        default:
                rb_raise(rb_eArgError, "unknown family: %d", family);
                break;
        }

        return GOBJ2RVAL_UNREF(g_inet_address_new_from_bytes(bytes, family));
}

static VALUE
rg_s_new_any(G_GNUC_UNUSED VALUE self, VALUE family)
{
        return GOBJ2RVAL_UNREF(g_inet_address_new_any(RVAL2GSOCKETFAMILY(family)));
}

static VALUE
rg_s_new_loopback(G_GNUC_UNUSED VALUE self, VALUE family)
{
        return GOBJ2RVAL_UNREF(g_inet_address_new_loopback(RVAL2GSOCKETFAMILY(family)));
}

/* TODO: Return array of bytes instead? */
static VALUE
rg_to_bytes(VALUE self)
{
        gsize size;

        size = g_inet_address_get_native_size(_SELF(self));
        return rb_str_new((const gchar *)g_inet_address_to_bytes(_SELF(self)),
                          size);
}

static VALUE
rg_get_native_size(VALUE self)
{
        return GSIZE2RVAL(g_inet_address_get_native_size(_SELF(self)));
}

static VALUE
rg_to_string(VALUE self)
{
        return CSTR2RVAL_FREE(g_inet_address_to_string(_SELF(self)));
}

void
Init_ginetaddress(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_INET_ADDRESS, "InetAddress", mGio);

        RG_DEF_SMETHOD(new_from_string, 1);
        RG_DEF_SMETHOD(new_from_bytes, -1);
        RG_DEF_SMETHOD(new_any, 1);
        RG_DEF_SMETHOD(new_loopback, 1);

        RG_DEF_METHOD(to_bytes, 0);
        RG_DEF_METHOD(get_native_size, 0);
        RG_DEF_METHOD(to_string, 0);
        RG_DEF_ALIAS("to_str", "to_string");
        RG_DEF_ALIAS("to_s", "to_string");
}
