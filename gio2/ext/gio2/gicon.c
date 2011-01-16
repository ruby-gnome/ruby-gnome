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

#define _SELF(value) RVAL2GICON(value)

static VALUE
icon_new_for_string(G_GNUC_UNUSED VALUE self, VALUE string)
{
        GError *error = NULL;
        GIcon *icon;

        icon = g_icon_new_for_string(RVAL2CSTR(string), &error);
        if (icon == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(icon);
}

static VALUE
icon_hash(VALUE self)
{
        return GUINT2RVAL(g_icon_hash(_SELF(self)));
}

static VALUE
icon_equal(VALUE self, VALUE other)
{
        return CBOOL2RVAL(g_icon_equal(_SELF(self), _SELF(other)));
}

static VALUE
icon_to_string(VALUE self)
{
        return CSTR2RVAL_FREE(g_icon_to_string(_SELF(self)));
}

void
Init_gicon(VALUE glib)
{
        VALUE icon = G_DEF_INTERFACE(G_TYPE_ICON, "Icon", glib);

        rb_define_singleton_method(icon, "new_for_string", icon_new_for_string, 1);

        rb_define_method(icon, "hash", icon_hash, 0);
        rb_define_method(icon, "==", icon_equal, 1);
        rb_define_method(icon, "to_string", icon_to_string, 0);
        rb_define_alias(icon, "to_s", "to_string");
}
