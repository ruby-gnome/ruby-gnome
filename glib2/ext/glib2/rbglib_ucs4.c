/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2019  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Kouhei Sutou
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
#include "rbglib.h"

#define RG_TARGET_NAMESPACE mUCS4

static VALUE
rg_s_to_utf16(G_GNUC_UNUSED VALUE self, VALUE rb_ucs4)
{
    VALUE result;
    gunichar *ucs4;
    gunichar2 *utf16;
    glong len, items_written;
    GError *error = NULL;

    ucs4 = (gunichar *)(void *)StringValuePtr(rb_ucs4);
    len = RSTRING_LEN(rb_ucs4) / sizeof(*ucs4);

    utf16 = g_ucs4_to_utf16(ucs4, len, NULL, &items_written, &error);

    if (error)
        RAISE_GERROR(error);

    result = CSTR2RVAL_LEN_UTF16((char *)utf16,
                                   items_written * sizeof(*utf16));
    g_free(utf16);
    return result;
}

static VALUE
rg_s_to_utf8(G_GNUC_UNUSED VALUE self, VALUE rb_ucs4)
{
    VALUE result;
    gunichar *ucs4;
    gchar *utf8;
    glong len, items_written;
    GError *error = NULL;

    ucs4 = (gunichar *)(void *)StringValuePtr(rb_ucs4);
    len = RSTRING_LEN(rb_ucs4) / sizeof(*ucs4);

    utf8 = g_ucs4_to_utf8(ucs4, len, NULL, &items_written, &error);

    if (error)
        RAISE_GERROR(error);

    result = CSTR2RVAL_LEN(utf8, items_written);
    g_free(utf8);
    return result;
}

void
Init_glib_ucs4(void)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGLib, "UCS4");

    RG_DEF_SMETHOD(to_utf16, 1);
    RG_DEF_SMETHOD(to_utf8, 1);
}
