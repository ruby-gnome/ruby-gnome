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

#define _SELF(value) RVAL2GINETADDRESS(value)

static VALUE
inetaddress_new_from_string(G_GNUC_UNUSED VALUE self, VALUE string)
{
        return GOBJ2RVAL_UNREF(g_inet_address_new_from_string(RVAL2CSTR(string)));
}

/* TODO: Should this take an array instead? */
static VALUE
inetaddress_new_from_bytes(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
        VALUE rbbytes, rbfamily;
        guint8 *bytes;
        GSocketFamily family;

        rb_scan_args(argc, argv, "11", &rbbytes, &rbfamily);
        bytes = (guint8 *)RVAL2CSTR(rbbytes);
        family = RVAL2GSOCKETFAMILY(rbfamily);

        /* TODO: Should check encoding. */
        /* TODO: We can’t pass bytes as such, as it will be NULL-terminated. */
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
inetaddress_new_any(G_GNUC_UNUSED VALUE self, VALUE family)
{
        return GOBJ2RVAL_UNREF(g_inet_address_new_any(RVAL2GSOCKETFAMILY(family)));
}

static VALUE
inetaddress_new_loopback(G_GNUC_UNUSED VALUE self, VALUE family)
{
        return GOBJ2RVAL_UNREF(g_inet_address_new_loopback(RVAL2GSOCKETFAMILY(family)));
}

/* TODO: Return array of bytes instead? */
static VALUE
inetaddress_to_bytes(VALUE self)
{
        gsize size;

        size = g_inet_address_get_native_size(_SELF(self));
        return rb_str_new((const gchar *)g_inet_address_to_bytes(_SELF(self)),
                          size);
}

static VALUE
inetaddress_get_native_size(VALUE self)
{
        return GSIZE2RVAL(g_inet_address_get_native_size(_SELF(self)));
}

static VALUE
inetaddress_to_string(VALUE self)
{
        return CSTR2RVAL_FREE(g_inet_address_to_string(_SELF(self)));
}

void
Init_ginetaddress(VALUE glib)
{
        VALUE inetaddress = G_DEF_INTERFACE(G_TYPE_INET_ADDRESS, "InetAddress", glib);

        rb_define_singleton_method(inetaddress, "new_from_string", inetaddress_new_from_string, 1);
        rb_define_singleton_method(inetaddress, "new_from_bytes", inetaddress_new_from_bytes, -1);
        rb_define_singleton_method(inetaddress, "new_any", inetaddress_new_any, 1);
        rb_define_singleton_method(inetaddress, "new_loopback", inetaddress_new_loopback, 1);

        rb_define_method(inetaddress, "to_bytes", inetaddress_to_bytes, 0);
        rb_define_method(inetaddress, "get_native_size", inetaddress_get_native_size, 0);
        rb_define_method(inetaddress, "to_string", inetaddress_to_string, 0);
        rb_define_alias(inetaddress, "to_str", "to_string");
        rb_define_alias(inetaddress, "to_s", "to_string");
}
