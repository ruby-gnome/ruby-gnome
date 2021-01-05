/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2020  Ruby-GNOME Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#include "rbgprivate.h"

static VALUE
strv_to_ruby(const GValue *from)
{
    return STRV2RVAL((const gchar **)g_value_get_boxed(from));
}

static void
strv_from_ruby(VALUE from, GValue *to)
{
    gchar **strings = RVAL2STRV(from);
    g_value_set_boxed(to, strings);
    g_free(strings);
}

void
Init_gobject_gstrv(void)
{
    /* GStrv is treated as Array */
    rbgobj_register_g2r_func(G_TYPE_STRV, strv_to_ruby);
    rbgobj_register_r2g_func(G_TYPE_STRV, strv_from_ruby);
}
