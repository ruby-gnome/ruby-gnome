/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE mUTF8

static VALUE
rg_s_get_char(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE utf8, validate;
    gunichar result;

    rb_scan_args(argc, argv, "11", &utf8, &validate);

    if (RVAL2CBOOL(validate)) {
        StringValue(utf8);
        result = g_utf8_get_char_validated(RSTRING_PTR(utf8),
                                           RSTRING_LEN(utf8));
        if (result == (gunichar)-1) {
            return INT2NUM(-1);
        } else if (result == (gunichar)-2) {
            return INT2NUM(-2);
        }
    } else {
        result = g_utf8_get_char(StringValueCStr(utf8));
    }

    return UINT2NUM(result);
}

static VALUE
rg_s_size(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
{
    gchar *utf8;

    utf8 = StringValueCStr(rb_utf8);
    return INT2NUM(g_utf8_strlen(utf8, RSTRING_LEN(rb_utf8)));
}

static VALUE
rg_s_reverse(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
{
    VALUE result;
    gchar *utf8, *reversed_utf8;

    utf8 = StringValueCStr(rb_utf8);
    reversed_utf8 = g_utf8_strreverse(utf8, RSTRING_LEN(rb_utf8));
    result = CSTR2RVAL(reversed_utf8);
    g_free(reversed_utf8);
    return result;
}

static VALUE
rg_s_validate(G_GNUC_UNUSED VALUE self, VALUE str)
{
    StringValue(str);
    return CBOOL2RVAL(g_utf8_validate(RSTRING_PTR(str), RSTRING_LEN(str),
                                      NULL));
}

static VALUE
rg_s_upcase(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
{
    VALUE result;
    gchar *utf8, *upcased_utf8;

    utf8 = StringValueCStr(rb_utf8);
    upcased_utf8 = g_utf8_strup(utf8, RSTRING_LEN(rb_utf8));
    result = CSTR2RVAL(upcased_utf8);
    g_free(upcased_utf8);
    return result;
}

static VALUE
rg_s_downcase(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
{
    VALUE result;
    gchar *utf8, *downcased_utf8;

    utf8 = StringValueCStr(rb_utf8);
    downcased_utf8 = g_utf8_strdown(utf8, RSTRING_LEN(rb_utf8));
    result = CSTR2RVAL(downcased_utf8);
    g_free(downcased_utf8);
    return result;
}

static VALUE
rg_s_casefold(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
{
    VALUE result;
    gchar *utf8, *casefolded_utf8;

    utf8 = StringValueCStr(rb_utf8);
    casefolded_utf8 = g_utf8_casefold(utf8, RSTRING_LEN(rb_utf8));
    result = CSTR2RVAL(casefolded_utf8);
    g_free(casefolded_utf8);
    return result;
}

static VALUE
rg_s_normalize(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE rb_utf8, rb_mode, result;
    gchar *utf8, *normalized_utf8;
    GNormalizeMode mode = G_NORMALIZE_DEFAULT;

    rb_scan_args(argc, argv, "11", &rb_utf8, &rb_mode);

    if (!NIL_P(rb_mode))
        mode = RVAL2GENUM(rb_mode, G_TYPE_NORMALIZE_MODE);

    utf8 = StringValueCStr(rb_utf8);
    normalized_utf8 = g_utf8_normalize(utf8, RSTRING_LEN(rb_utf8), mode);
    result = CSTR2RVAL(normalized_utf8);
    g_free(normalized_utf8);
    return result;
}

static VALUE
rg_s_collate(G_GNUC_UNUSED VALUE self, VALUE utf8a, VALUE utf8b)
{
    return INT2NUM(g_utf8_collate(StringValueCStr(utf8a),
                                  StringValueCStr(utf8b)));
}

static VALUE
rg_s_collate_key(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE result, rb_utf8, for_filename;
    gchar *key, *utf8;
    gssize len;

    rb_scan_args(argc, argv, "11", &rb_utf8, &for_filename);

    utf8 = StringValueCStr(rb_utf8);
    len = RSTRING_LEN(rb_utf8);
    if (RVAL2CBOOL(for_filename))
        key = g_utf8_collate_key_for_filename(utf8, len);
    else
        key = g_utf8_collate_key(utf8, len);

    result = CSTR2RVAL(key);
    g_free(key);
    return result;
}

static VALUE
rg_s_to_utf16(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
{
    VALUE result;
    gchar *utf8;
    gunichar2 *utf16;
    glong len, items_written;
    GError *error = NULL;

    utf8 = StringValueCStr(rb_utf8);
    len = RSTRING_LEN(rb_utf8);

    utf16 = g_utf8_to_utf16(utf8, len, NULL, &items_written, &error);

    if (error)
        RAISE_GERROR(error);

    result = CSTR2RVAL_LEN_UTF16((char *)utf16,
                                   items_written * sizeof(*utf16));
    g_free(utf16);
    return result;
}

static VALUE
rg_s_to_ucs4(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE result, rb_utf8, is_fast;
    gchar *utf8;
    gunichar *ucs4;
    glong len, items_written;

    rb_scan_args(argc, argv, "11", &rb_utf8, &is_fast);

    utf8 = StringValueCStr(rb_utf8);
    len = RSTRING_LEN(rb_utf8);

    if (RVAL2CBOOL(is_fast)) {
        ucs4 = g_utf8_to_ucs4_fast(utf8, len, &items_written);
    } else {
        GError *error = NULL;
        ucs4 = g_utf8_to_ucs4(utf8, len, NULL, &items_written, &error);

        if (error)
            RAISE_GERROR(error);
    }

    result = CSTR2RVAL_LEN_UCS4((char *)ucs4, items_written * sizeof(*ucs4));
    g_free(ucs4);
    return result;
}

void
Init_glib_utf8(void)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGLib, "UTF8");

    /*
      Not implemented.
      g_utf8_next_char
    */
    RG_DEF_SMETHOD(get_char, -1);
    /*
      Not implemented.
      g_utf8_offset_to_pointer
      g_utf8_pointer_to_offset
      g_utf8_prev_char
      g_utf8_find_next_char
      g_utf8_find_prev_char
      g_utf8_prev_char
    */
    RG_DEF_SMETHOD(size, 1);
    /*
      Not implemented.
      g_utf8_strncpy
      g_utf8_strrchr
    */
    RG_DEF_SMETHOD(reverse, 1);
    RG_DEF_SMETHOD(validate, 1);

    RG_DEF_SMETHOD(upcase, 1);
    RG_DEF_SMETHOD(downcase, 1);
    RG_DEF_SMETHOD(casefold, 1);

    RG_DEF_SMETHOD(normalize, -1);

    RG_DEF_SMETHOD(collate, 2);
    RG_DEF_SMETHOD(collate_key, -1);

    RG_DEF_SMETHOD(to_utf16, 1);
    RG_DEF_SMETHOD(to_ucs4, -1);
}
