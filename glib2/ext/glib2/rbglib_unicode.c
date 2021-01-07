/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE mUnicode

static VALUE
rbglib_m_charset(G_GNUC_UNUSED VALUE self)
{
    const char *charset;
    g_get_charset(&charset);
    return CSTR2RVAL(charset);
}

static VALUE
rg_s_canonical_ordering(G_GNUC_UNUSED VALUE self, VALUE rb_ucs4)
{
    VALUE normalized_ucs4;
    gchar *original_str;
    gunichar *ucs4;
    long len;

    original_str = StringValuePtr(rb_ucs4);
    len = RSTRING_LEN(rb_ucs4);
    ucs4 = g_memdup(original_str, (guint)len);
    g_unicode_canonical_ordering(ucs4, len);
    normalized_ucs4 = CSTR2RVAL_LEN_UCS4((const char *)ucs4, len);
    g_free(ucs4);
    return normalized_ucs4;
}

void
Init_glib_unicode(void)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGLib, "Unicode");

    /* GUnicodeType */
    G_DEF_CLASS(G_TYPE_UNICODE_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_UNICODE_TYPE, "G_UNICODE_");
    /* GUnicodeBreakType */
    G_DEF_CLASS(G_TYPE_UNICODE_BREAK_TYPE, "BreakType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_UNICODE_BREAK_TYPE, "G_UNICODE_");

    /* GUnicodeScript */
    G_DEF_CLASS(G_TYPE_UNICODE_SCRIPT, "Script", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_UNICODE_SCRIPT, "G_UNICODE_");

    G_DEF_CLASS(G_TYPE_NORMALIZE_MODE, "NormalizeMode", mGLib);

    rbg_define_singleton_method(mGLib, "charset", rbglib_m_charset, 0);

    RG_DEF_SMETHOD(canonical_ordering, 1);
}
