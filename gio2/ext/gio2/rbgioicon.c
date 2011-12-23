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

#define RG_TARGET_NAMESPACE mIcon
#define _SELF(value) RVAL2GICON(value)

static VALUE
rg_s_new_for_string(G_GNUC_UNUSED VALUE self, VALUE string)
{
        GError *error = NULL;
        GIcon *icon;

        icon = g_icon_new_for_string(RVAL2CSTR(string), &error);
        if (icon == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(icon);
}

static VALUE
rg_hash(VALUE self)
{
        return GUINT2RVAL(g_icon_hash(_SELF(self)));
}

static VALUE
rg_operator_icon_equal(VALUE self, VALUE other)
{
        return CBOOL2RVAL(g_icon_equal(_SELF(self), _SELF(other)));
}

static VALUE
rg_to_string(VALUE self)
{
        return CSTR2RVAL_FREE(g_icon_to_string(_SELF(self)));
}

void
Init_gicon(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_ICON, "Icon", mGio);

        RG_DEF_SMETHOD(new_for_string, 1);

        RG_DEF_METHOD(hash, 0);
        RG_DEF_METHOD_OPERATOR("==", icon_equal, 1);
        RG_DEF_METHOD(to_string, 0);
        RG_DEF_ALIAS("to_s", "to_string");
}
