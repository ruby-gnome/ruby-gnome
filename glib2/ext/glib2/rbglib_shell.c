/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE mShell

static VALUE
rg_s_parse(G_GNUC_UNUSED VALUE self, VALUE command_line)
{
    gint argc;
    gchar **argv;
    GError *error = NULL;

    if (!g_shell_parse_argv(RVAL2CSTR(command_line), &argc, &argv, &error))
        RAISE_GERROR(error);

    return STRV2RVAL_FREE(argv);
}

static VALUE
rg_s_quote(G_GNUC_UNUSED VALUE self, VALUE unquoted_string)
{
    return CSTR2RVAL_FREE(g_shell_quote(RVAL2CSTR(unquoted_string)));
}

static VALUE
rg_s_unquote(G_GNUC_UNUSED VALUE self, VALUE quoted_string)
{
    GError *error = NULL;
    gchar *str = g_shell_unquote(RVAL2CSTR(quoted_string), &error);
    if (str == NULL)
        RAISE_GERROR(error);

    return CSTR2RVAL_FREE(str);
}

void
Init_glib_shell(void)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGLib, "Shell");

    RG_DEF_SMETHOD(parse, 1);
    RG_DEF_SMETHOD(quote, 1);
    RG_DEF_SMETHOD(unquote, 1);
}
