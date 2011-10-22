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

static VALUE
rbg_ucs4_to_rval_len(const gchar* ucs4, gsize len)
{
    const gchar *ucs4_encoding;
#if  G_BYTE_ORDER == G_LITTLE_ENDIAN
    ucs4_encoding = "UTF-32LE";
#else
    ucs4_encoding = "UTF-32BE";
#endif

    return CSTR2RVAL_LEN_ENC(ucs4, len, ucs4_encoding);
}

static VALUE
rbg_utf16_to_rval_len(const gchar* utf16, gsize len)
{
    const gchar *utf16_encoding;
#if  G_BYTE_ORDER == G_LITTLE_ENDIAN
    utf16_encoding = "UTF-16LE";
#else
    utf16_encoding = "UTF-16BE";
#endif

    return CSTR2RVAL_LEN_ENC(utf16, len, utf16_encoding);
}

static VALUE
rbglib_m_charset(G_GNUC_UNUSED VALUE self)
{
    const char *charset;
    g_get_charset(&charset);
    return CSTR2RVAL(charset);
}

#define DEF_IS_UNICHAR(name)                                         \
static VALUE                                                         \
rbglib_m_unichar_is ## name(G_GNUC_UNUSED VALUE self, VALUE unichar) \
{                                                                    \
    return CBOOL2RVAL(g_unichar_is ## name(NUM2UINT(unichar)));      \
}

DEF_IS_UNICHAR(alnum)
DEF_IS_UNICHAR(alpha)
DEF_IS_UNICHAR(cntrl)
DEF_IS_UNICHAR(digit)
DEF_IS_UNICHAR(graph)
DEF_IS_UNICHAR(lower)
DEF_IS_UNICHAR(print)
DEF_IS_UNICHAR(punct)
DEF_IS_UNICHAR(space)
DEF_IS_UNICHAR(upper)
DEF_IS_UNICHAR(xdigit)
DEF_IS_UNICHAR(title)
DEF_IS_UNICHAR(defined)
DEF_IS_UNICHAR(wide)
#if GLIB_CHECK_VERSION(2,12,0)
DEF_IS_UNICHAR(wide_cjk)
#endif

#undef DEF_IS_UNICHAR

static VALUE
rbglib_m_unichar_toupper(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return UINT2NUM(g_unichar_toupper(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_tolower(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return UINT2NUM(g_unichar_tolower(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_totitle(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return UINT2NUM(g_unichar_totitle(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_digit_value(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return INT2NUM(g_unichar_digit_value(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_xdigit_value(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return INT2NUM(g_unichar_xdigit_value(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_type(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return GENUM2RVAL(g_unichar_type(NUM2UINT(unichar)),
                      G_TYPE_UNICODE_TYPE);
}

static VALUE
rbglib_m_unichar_break_type(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return GENUM2RVAL(g_unichar_break_type(NUM2UINT(unichar)),
                      G_TYPE_UNICODE_BREAK_TYPE);
}

static VALUE
rbglib_m_unicode_canonical_ordering(G_GNUC_UNUSED VALUE self, VALUE rb_ucs4)
{
    VALUE normalized_ucs4;
    gchar *original_str;
    gunichar *ucs4;
    gint len;

    original_str = StringValuePtr(rb_ucs4);
    len = RSTRING_LEN(rb_ucs4);
    ucs4 = g_memdup(original_str, len);
    g_unicode_canonical_ordering(ucs4, len);
    normalized_ucs4 = rbg_ucs4_to_rval_len((const char *)ucs4, len);
    g_free(ucs4);
    return normalized_ucs4;
}

static VALUE
rbglib_m_unicode_canonical_decomposition(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    VALUE normalized_ucs4;
    gunichar *ucs4;
    gsize len;

    ucs4 = g_unicode_canonical_decomposition(NUM2UINT(unichar), &len);
    normalized_ucs4 = rbg_ucs4_to_rval_len((const char *)ucs4,
                                           len * sizeof(gunichar));
    g_free(ucs4);
    return normalized_ucs4;
}

#if GLIB_CHECK_VERSION(2,4,0)
static VALUE
rbglib_m_unichar_get_mirror_char(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    gunichar mirrored_char;

    if (g_unichar_get_mirror_char(NUM2UINT(unichar), &mirrored_char)) {
        return UINT2NUM(mirrored_char);
    } else {
        return unichar;
    }
}
#endif

#if GLIB_CHECK_VERSION(2,14,0)
static VALUE
rbglib_m_unichar_combining_class(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return INT2NUM(g_unichar_combining_class(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_get_script(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return GENUM2RVAL(g_unichar_get_script(NUM2UINT(unichar)),
                      G_TYPE_UNICODE_SCRIPT);
}

static VALUE
rbglib_m_unichar_ismark(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return CBOOL2RVAL(g_unichar_ismark(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_iszerowidth(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return CBOOL2RVAL(g_unichar_iszerowidth(NUM2UINT(unichar)));
}
#endif

static VALUE
rbglib_m_utf8_get_char(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
rbglib_m_utf8_strlen(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
{
    gchar *utf8;

    utf8 = StringValueCStr(rb_utf8);
    return INT2NUM(g_utf8_strlen(utf8, RSTRING_LEN(rb_utf8)));
}

static VALUE
rbglib_m_utf8_strreverse(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
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
rbglib_m_utf8_validate(G_GNUC_UNUSED VALUE self, VALUE str)
{
    StringValue(str);
    return CBOOL2RVAL(g_utf8_validate(RSTRING_PTR(str), RSTRING_LEN(str),
                                      NULL));
}

static VALUE
rbglib_m_utf8_strup(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
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
rbglib_m_utf8_strdown(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
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
rbglib_m_utf8_casefold(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
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
rbglib_m_utf8_normalize(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
rbglib_m_utf8_collate(G_GNUC_UNUSED VALUE self, VALUE utf8a, VALUE utf8b)
{
    return INT2NUM(g_utf8_collate(StringValueCStr(utf8a),
                                  StringValueCStr(utf8b)));
}

static VALUE
rbglib_m_utf8_collate_key(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE result, rb_utf8, for_filename;
    gchar *key, *utf8;
    gssize len;

    rb_scan_args(argc, argv, "11", &rb_utf8, &for_filename);

    utf8 = StringValueCStr(rb_utf8);
    len = RSTRING_LEN(rb_utf8);
#if GLIB_CHECK_VERSION(2,8,0)
    if (RVAL2CBOOL(for_filename))
        key = g_utf8_collate_key_for_filename(utf8, len);
    else
#endif
        key = g_utf8_collate_key(utf8, len);

    result = CSTR2RVAL(key);
    g_free(key);
    return result;
}

static VALUE
rbglib_m_utf8_to_utf16(G_GNUC_UNUSED VALUE self, VALUE rb_utf8)
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

    result = rbg_utf16_to_rval_len((char *)utf16,
                                   items_written * sizeof(*utf16));
    g_free(utf16);
    return result;
}

static VALUE
rbglib_m_utf8_to_ucs4(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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

    result = rbg_ucs4_to_rval_len((char *)ucs4, items_written * sizeof(*ucs4));
    g_free(ucs4);
    return result;
}

static VALUE
rbglib_m_utf16_to_ucs4(G_GNUC_UNUSED VALUE self, VALUE rb_utf16)
{
    VALUE result;
    gunichar *ucs4;
    gunichar2 *utf16;
    glong len, items_written;
    GError *error = NULL;

    utf16 = (gunichar2 *)StringValueCStr(rb_utf16);
    len = RSTRING_LEN(rb_utf16) / sizeof(*utf16);

    ucs4 = g_utf16_to_ucs4(utf16, len, NULL, &items_written, &error);

    if (error)
        RAISE_GERROR(error);

    result = rbg_ucs4_to_rval_len((char *)ucs4, items_written * sizeof(*ucs4));
    g_free(ucs4);
    return result;
}

static VALUE
rbglib_m_utf16_to_utf8(G_GNUC_UNUSED VALUE self, VALUE rb_utf16)
{
    VALUE result;
    gchar *utf8;
    gunichar2 *utf16;
    glong len, items_written;
    GError *error = NULL;

    utf16 = (gunichar2 *)StringValueCStr(rb_utf16);
    len = RSTRING_LEN(rb_utf16) / sizeof(*utf16);

    utf8 = g_utf16_to_utf8(utf16, len, NULL, &items_written, &error);

    if (error)
        RAISE_GERROR(error);

    result = CSTR2RVAL_LEN(utf8, items_written * sizeof(*utf8));
    g_free(utf8);
    return result;
}

static VALUE
rbglib_m_ucs4_to_utf16(G_GNUC_UNUSED VALUE self, VALUE rb_ucs4)
{
    VALUE result;
    gunichar *ucs4;
    gunichar2 *utf16;
    glong len, items_written;
    GError *error = NULL;

    ucs4 = (gunichar *)StringValuePtr(rb_ucs4);
    len = RSTRING_LEN(rb_ucs4) / sizeof(*ucs4);

    utf16 = g_ucs4_to_utf16(ucs4, len, NULL, &items_written, &error);

    if (error)
        RAISE_GERROR(error);

    result = rbg_utf16_to_rval_len((char *)utf16,
                                   items_written * sizeof(*utf16));
    g_free(utf16);
    return result;
}

static VALUE
rbglib_m_ucs4_to_utf8(G_GNUC_UNUSED VALUE self, VALUE rb_ucs4)
{
    VALUE result;
    gunichar *ucs4;
    gchar *utf8;
    glong len, items_written;
    GError *error = NULL;

    ucs4 = (gunichar *)StringValuePtr(rb_ucs4);
    len = RSTRING_LEN(rb_ucs4) / sizeof(*ucs4);

    utf8 = g_ucs4_to_utf8(ucs4, len, NULL, &items_written, &error);

    if (error)
        RAISE_GERROR(error);

    result = CSTR2RVAL_LEN(utf8, items_written);
    g_free(utf8);
    return result;
}

static VALUE
rbglib_m_unichar_to_utf8(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    gchar utf8[6];
    gint len;

    len = g_unichar_to_utf8(NUM2UINT(unichar), utf8);
    return CSTR2RVAL_LEN(utf8, len);
}

void
Init_glib_unicode(void)
{
    VALUE mGLibUniChar, mGLibUnicode, mGLibUTF8, mGLibUTF16, mGLibUCS4;

    mGLibUniChar = rb_define_module_under(mGLib, "UniChar");
    mGLibUnicode = rb_define_module_under(mGLib, "Unicode");
    mGLibUTF8 = rb_define_module_under(mGLib, "UTF8");
    mGLibUTF16 = rb_define_module_under(mGLib, "UTF16");
    mGLibUCS4 = rb_define_module_under(mGLib, "UCS4");

    /* GUnicodeType */
    G_DEF_CLASS(G_TYPE_UNICODE_TYPE, "Type", mGLibUnicode);
    G_DEF_CONSTANTS(mGLibUnicode, G_TYPE_UNICODE_TYPE, "G_UNICODE_");
    /* GUnicodeBreakType */
    G_DEF_CLASS(G_TYPE_UNICODE_BREAK_TYPE, "BreakType", mGLibUnicode);
    G_DEF_CONSTANTS(mGLibUnicode, G_TYPE_UNICODE_BREAK_TYPE, "G_UNICODE_");

#if GLIB_CHECK_VERSION(2,14,0)
    /* GUnicodeScript */
    G_DEF_CLASS(G_TYPE_UNICODE_SCRIPT, "Script", mGLibUnicode);
    G_DEF_CONSTANTS(mGLibUnicode, G_TYPE_UNICODE_SCRIPT, "G_UNICODE_");
#endif

    G_DEF_CLASS(G_TYPE_NORMALIZE_MODE, "NormalizeMode", mGLib);

    rb_define_module_function(mGLib, "charset", rbglib_m_charset, 0);

    rb_define_module_function(mGLibUniChar, "alnum?",
                              rbglib_m_unichar_isalnum, 1);
    rb_define_module_function(mGLibUniChar, "alpha?",
                              rbglib_m_unichar_isalpha, 1);
    rb_define_module_function(mGLibUniChar, "cntrl?",
                              rbglib_m_unichar_iscntrl, 1);
    rb_define_module_function(mGLibUniChar, "digit?",
                              rbglib_m_unichar_isdigit, 1);
    rb_define_module_function(mGLibUniChar, "graph?",
                              rbglib_m_unichar_isgraph, 1);
    rb_define_module_function(mGLibUniChar, "lower?",
                              rbglib_m_unichar_islower, 1);
    rb_define_module_function(mGLibUniChar, "print?",
                              rbglib_m_unichar_isprint, 1);
    rb_define_module_function(mGLibUniChar, "punct?",
                              rbglib_m_unichar_ispunct, 1);
    rb_define_module_function(mGLibUniChar, "space?",
                              rbglib_m_unichar_isspace, 1);
    rb_define_module_function(mGLibUniChar, "upper?",
                              rbglib_m_unichar_isupper, 1);
    rb_define_module_function(mGLibUniChar, "xdigit?",
                              rbglib_m_unichar_isxdigit, 1);
    rb_define_module_function(mGLibUniChar, "title?",
                              rbglib_m_unichar_istitle, 1);
    rb_define_module_function(mGLibUniChar, "defined?",
                              rbglib_m_unichar_isdefined, 1);
    rb_define_module_function(mGLibUniChar, "wide?",
                              rbglib_m_unichar_iswide, 1);
#if GLIB_CHECK_VERSION(2,12,0)
    rb_define_module_function(mGLibUniChar, "wide_cjk?",
                              rbglib_m_unichar_iswide_cjk, 1);
#endif

    rb_define_module_function(mGLibUniChar, "to_upper",
                              rbglib_m_unichar_toupper, 1);
    rb_define_module_function(mGLibUniChar, "to_lower",
                              rbglib_m_unichar_tolower, 1);
    rb_define_module_function(mGLibUniChar, "to_title",
                              rbglib_m_unichar_totitle, 1);

    rb_define_module_function(mGLibUniChar, "digit_value",
                              rbglib_m_unichar_digit_value, 1);
    rb_define_module_function(mGLibUniChar, "xdigit_value",
                              rbglib_m_unichar_xdigit_value, 1);

    rb_define_module_function(mGLibUniChar, "type", rbglib_m_unichar_type, 1);
    rb_define_module_function(mGLibUniChar, "break_type",
                              rbglib_m_unichar_break_type, 1);

    rb_define_singleton_method(mGLibUnicode, "canonical_ordering",
                               rbglib_m_unicode_canonical_ordering, 1);
    rb_define_singleton_method(mGLibUnicode, "canonical_decomposition",
                               rbglib_m_unicode_canonical_decomposition, 1);

#if GLIB_CHECK_VERSION(2,4,0)
    rb_define_module_function(mGLibUniChar, "get_mirror_char",
                              rbglib_m_unichar_get_mirror_char, 1);
#endif

#if GLIB_CHECK_VERSION(2,14,0)
    rb_define_module_function(mGLibUniChar, "combining_class",
			      rbglib_m_unichar_combining_class, 1);
    rb_define_module_function(mGLibUniChar, "get_script",
                              rbglib_m_unichar_get_script, 1);
    rb_define_module_function(mGLibUniChar, "mark?",
			      rbglib_m_unichar_ismark, 1);
    rb_define_module_function(mGLibUniChar, "zero_width?",
			      rbglib_m_unichar_iszerowidth, 1);
#endif

    /*
      Not implemented.
      g_utf8_next_char
    */
    rb_define_module_function(mGLibUTF8, "get_char",
                              rbglib_m_utf8_get_char, -1);
    /*
      Not implemented.
      g_utf8_offset_to_pointer
      g_utf8_pointer_to_offset
      g_utf8_prev_char
      g_utf8_find_next_char
      g_utf8_find_prev_char
      g_utf8_prev_char
    */
    rb_define_module_function(mGLibUTF8, "size", rbglib_m_utf8_strlen, 1);
    /*
      Not implemented.
      g_utf8_strncpy
      g_utf8_strrchr
    */
    rb_define_module_function(mGLibUTF8, "reverse",
                              rbglib_m_utf8_strreverse, 1);
    rb_define_module_function(mGLibUTF8, "validate",
                              rbglib_m_utf8_validate, 1);

    rb_define_module_function(mGLibUTF8, "upcase",
                              rbglib_m_utf8_strup, 1);
    rb_define_module_function(mGLibUTF8, "downcase",
                              rbglib_m_utf8_strdown, 1);
    rb_define_module_function(mGLibUTF8, "casefold",
                              rbglib_m_utf8_casefold, 1);

    rb_define_module_function(mGLibUTF8, "normalize",
                              rbglib_m_utf8_normalize, -1);

    rb_define_module_function(mGLibUTF8, "collate",
                              rbglib_m_utf8_collate, 2);
    rb_define_module_function(mGLibUTF8, "collate_key",
                              rbglib_m_utf8_collate_key, -1);

    rb_define_module_function(mGLibUTF8, "to_utf16",
                              rbglib_m_utf8_to_utf16, 1);
    rb_define_module_function(mGLibUTF8, "to_ucs4",
                              rbglib_m_utf8_to_ucs4, -1);

    rb_define_module_function(mGLibUTF16, "to_ucs4",
                              rbglib_m_utf16_to_ucs4, 1);
    rb_define_module_function(mGLibUTF16, "to_utf8",
                              rbglib_m_utf16_to_utf8, 1);

    rb_define_module_function(mGLibUCS4, "to_utf16",
                              rbglib_m_ucs4_to_utf16, 1);
    rb_define_module_function(mGLibUCS4, "to_utf8",
                              rbglib_m_ucs4_to_utf8, 1);

    rb_define_module_function(mGLibUniChar, "to_utf8",
                              rbglib_m_unichar_to_utf8, 1);
}
