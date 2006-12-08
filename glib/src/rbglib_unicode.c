/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib_unicode.c -

  $Author: ktou $
  $Date: 2006/12/08 14:48:39 $

  Copyright (C) 2006 Kouhei Sutou

**********************************************************************/
#include "global.h"
#include "rbglib.h"
#include "glib-enum-types.h"

static VALUE
rbglib_m_charset(VALUE self)
{
    const char *charset;
    g_get_charset(&charset);
    return rb_str_new2(charset);
}

#define DEF_IS_UNICHAR(name)                                    \
static VALUE                                                    \
rbglib_m_unichar_is ## name(VALUE self, VALUE unichar)          \
{                                                               \
    return CBOOL2RVAL(g_unichar_is ## name(NUM2UINT(unichar))); \
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
DEF_IS_UNICHAR(wide_cjk)

#undef DEF_IS_UNICHAR

static VALUE
rbglib_m_unichar_toupper(VALUE self, VALUE unichar)
{
    return UINT2NUM(g_unichar_toupper(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_tolower(VALUE self, VALUE unichar)
{
    return UINT2NUM(g_unichar_tolower(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_totitle(VALUE self, VALUE unichar)
{
    return UINT2NUM(g_unichar_totitle(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_digit_value(VALUE self, VALUE unichar)
{
    return INT2NUM(g_unichar_digit_value(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_xdigit_value(VALUE self, VALUE unichar)
{
    return INT2NUM(g_unichar_xdigit_value(NUM2UINT(unichar)));
}

static VALUE
rbglib_m_unichar_type(VALUE self, VALUE unichar)
{
    return GENUM2RVAL(g_unichar_type(NUM2UINT(unichar)),
                      G_TYPE_UNICODE_TYPE);
}

static VALUE
rbglib_m_unichar_break_type(VALUE self, VALUE unichar)
{
    return GENUM2RVAL(g_unichar_break_type(NUM2UINT(unichar)),
                      G_TYPE_UNICODE_BREAK_TYPE);
}

static VALUE
rbglib_m_unicode_canonical_ordering(VALUE self, VALUE rb_ucs4)
{
    VALUE normalized_ucs4;
    gchar *original_str;
    gunichar *ucs4;
    gint len;

    original_str = StringValuePtr(rb_ucs4);
    len = RSTRING(rb_ucs4)->len;
    ucs4 = g_memdup(original_str, len);
    g_unicode_canonical_ordering(ucs4, len);
    normalized_ucs4 = rb_str_new((const char *)ucs4, len);
    g_free(ucs4);
    return normalized_ucs4;
}

static VALUE
rbglib_m_unicode_canonical_decomposition(VALUE self, VALUE unichar)
{
    VALUE normalized_ucs4;
    gunichar *ucs4;
    gsize len;

    ucs4 = g_unicode_canonical_decomposition(NUM2UINT(unichar), &len);
    normalized_ucs4 = rb_str_new((const char *)ucs4, len * sizeof(gunichar));
    g_free(ucs4);
    return normalized_ucs4;
}

static VALUE
rbglib_m_unichar_get_mirror_char(VALUE self, VALUE unichar)
{
    gunichar mirrored_char;

    if (g_unichar_get_mirror_char(NUM2UINT(unichar), &mirrored_char)) {
        return UINT2NUM(mirrored_char);
    } else {
        return unichar;
    }
}

#if GLIB_CHECK_VERSION(2,14,0)
static VALUE
rbglib_m_unichar_get_script(VALUE self, VALUE unichar)
{
    return GENUM2RVAL(g_unichar_get_script(NUM2UINT(unichar)),
                      G_TYPE_UNICODE_SCRIPT_TYPE);
}
#endif

static VALUE
rbglib_m_utf8_get_char(VALUE self, VALUE utf8)
{
    return UINT2NUM(g_utf8_get_char(StringValueCStr(utf8)));
}

static VALUE
rbglib_m_utf8_get_char_validated(VALUE self, VALUE utf8)
{
    gunichar result;

    StringValue(utf8);
    result = g_utf8_get_char_validated(RSTRING(utf8)->ptr,
                                       RSTRING(utf8)->len);
    if (result == (gunichar)-1) {
        return INT2NUM(-1);
    } else if (result == (gunichar)-2) {
        return INT2NUM(-2);
    } else {
        return UINT2NUM(result);
    }
}

static VALUE
rbglib_m_utf8_strlen(VALUE self, VALUE rb_utf8)
{
    gchar *utf8;

    utf8 = StringValueCStr(rb_utf8);
    return INT2NUM(g_utf8_strlen(utf8, RSTRING(rb_utf8)->len));
}

static VALUE
rbglib_m_utf8_validate(VALUE self, VALUE str)
{
    StringValue(str);
    return CBOOL2RVAL(g_utf8_validate(RSTRING(str)->ptr, RSTRING(str)->len,
                                      NULL));
}

static VALUE
rbglib_m_utf8_strup(VALUE self, VALUE rb_utf8)
{
    VALUE result;
    gchar *utf8, *upcased_utf8;

    utf8 = StringValueCStr(rb_utf8);
    upcased_utf8 = g_utf8_strup(utf8, RSTRING(rb_utf8)->len);
    result = rb_str_new2(upcased_utf8);
    g_free(upcased_utf8);
    return result;
}

static VALUE
rbglib_m_utf8_strdown(VALUE self, VALUE rb_utf8)
{
    VALUE result;
    gchar *utf8, *downcased_utf8;

    utf8 = StringValueCStr(rb_utf8);
    downcased_utf8 = g_utf8_strdown(utf8, RSTRING(rb_utf8)->len);
    result = rb_str_new2(downcased_utf8);
    g_free(downcased_utf8);
    return result;
}

static VALUE
rbglib_m_utf8_casefold(VALUE self, VALUE rb_utf8)
{
    VALUE result;
    gchar *utf8, *casefolded_utf8;

    utf8 = StringValueCStr(rb_utf8);
    casefolded_utf8 = g_utf8_casefold(utf8, RSTRING(rb_utf8)->len);
    result = rb_str_new2(casefolded_utf8);
    g_free(casefolded_utf8);
    return result;
}

static VALUE
rbglib_m_utf8_normalize(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_utf8, rb_mode, result;
    gchar *utf8, *normalized_utf8;
    GNormalizeMode mode = G_NORMALIZE_DEFAULT;

    rb_scan_args(argc, argv, "11", &rb_utf8, &rb_mode);

    if (!NIL_P(rb_mode))
        mode = RVAL2GENUM(rb_mode, G_TYPE_NORMALIZE_MODE);

    utf8 = StringValueCStr(rb_utf8);
    normalized_utf8 = g_utf8_normalize(utf8, RSTRING(rb_utf8)->len, mode);
    result = rb_str_new2(normalized_utf8);
    g_free(normalized_utf8);
    return result;
}


void
Init_glib_unicode(void)
{
    G_DEF_CLASS(G_TYPE_UNICODE_TYPE, "Unicode", mGLib);
    G_DEF_CONSTANTS(mGLib, G_TYPE_UNICODE_TYPE, "G_");

    G_DEF_CLASS(G_TYPE_UNICODE_BREAK_TYPE, "UnicodeBreak", mGLib);
    G_DEF_CONSTANTS(mGLib, G_TYPE_UNICODE_BREAK_TYPE, "G_");

#if GLIB_CHECK_VERSION(2,14,0)
    G_DEF_CLASS(G_TYPE_UNICODE_SCRIPT_TYPE, "UnicodeScript", mGLib);
    G_DEF_CONSTANTS(mGLib, G_TYPE_UNICODE_SCRIPT_TYPE, "G_");
#endif

    G_DEF_CLASS(G_TYPE_NORMALIZE_MODE, "NormalizeMode", mGLib);
    G_DEF_CONSTANTS(mGLib, G_TYPE_NORMALIZE_MODE, "G_");

    rb_define_module_function(mGLib, "charset", rbglib_m_charset, 0);

    rb_define_module_function(mGLib, "unichar_alnum?",
                              rbglib_m_unichar_isalnum, 1);
    rb_define_module_function(mGLib, "unichar_alpha?",
                              rbglib_m_unichar_isalpha, 1);
    rb_define_module_function(mGLib, "unichar_control?",
                              rbglib_m_unichar_iscntrl, 1);
    rb_define_module_function(mGLib, "unichar_digit?",
                              rbglib_m_unichar_isdigit, 1);
    rb_define_module_function(mGLib, "unichar_graph?",
                              rbglib_m_unichar_isgraph, 1);
    rb_define_module_function(mGLib, "unichar_lower?",
                              rbglib_m_unichar_islower, 1);
    rb_define_module_function(mGLib, "unichar_print?",
                              rbglib_m_unichar_isprint, 1);
    rb_define_module_function(mGLib, "unichar_punct?",
                              rbglib_m_unichar_ispunct, 1);
    rb_define_module_function(mGLib, "unichar_space?",
                              rbglib_m_unichar_isspace, 1);
    rb_define_module_function(mGLib, "unichar_upper?",
                              rbglib_m_unichar_isupper, 1);
    rb_define_module_function(mGLib, "unichar_xdigit?",
                              rbglib_m_unichar_isxdigit, 1);
    rb_define_module_function(mGLib, "unichar_title?",
                              rbglib_m_unichar_istitle, 1);
    rb_define_module_function(mGLib, "unichar_defined?",
                              rbglib_m_unichar_isdefined, 1);
    rb_define_module_function(mGLib, "unichar_wide?",
                              rbglib_m_unichar_iswide, 1);
    rb_define_module_function(mGLib, "unichar_wide_cjk?",
                              rbglib_m_unichar_iswide_cjk, 1);

    rb_define_module_function(mGLib, "unichar_to_upper",
                              rbglib_m_unichar_toupper, 1);
    rb_define_module_function(mGLib, "unichar_to_lower",
                              rbglib_m_unichar_tolower, 1);
    rb_define_module_function(mGLib, "unichar_to_title",
                              rbglib_m_unichar_totitle, 1);

    rb_define_module_function(mGLib, "unichar_digit_value",
                              rbglib_m_unichar_digit_value, 1);
    rb_define_module_function(mGLib, "unichar_xdigit_value",
                              rbglib_m_unichar_xdigit_value, 1);

    rb_define_module_function(mGLib, "unichar_type", rbglib_m_unichar_type, 1);
    rb_define_module_function(mGLib, "unichar_break_type",
                              rbglib_m_unichar_break_type, 1);

    rb_define_module_function(mGLib, "unicode_canonical_ordering",
                              rbglib_m_unicode_canonical_ordering, 1);
    rb_define_module_function(mGLib, "unicode_canonical_decomposition",
                              rbglib_m_unicode_canonical_decomposition, 1);

    rb_define_module_function(mGLib, "unichar_get_mirror_char",
                              rbglib_m_unichar_get_mirror_char, 1);

#if GLIB_CHECK_VERSION(2,14,0)
    /* I want't to use "unichar_script" because unichar_type
       and unichar_break_type don't have 'get'. - kou */
    rb_define_module_function(mGLib, "unichar_get_script",
                              rbglib_m_unichar_get_script, 1);
#endif

    /*
      Not implemented.
      g_utf8_next_char
    */
    rb_define_module_function(mGLib, "utf8_get_char",
                              rbglib_m_utf8_get_char, 1);
    rb_define_module_function(mGLib, "utf8_get_char_validated",
                              rbglib_m_utf8_get_char_validated, 1);
    /*
      Not implemented.
      g_utf8_offset_to_pointer
      g_utf8_pointer_to_offset
      g_utf8_prev_char
      g_utf8_find_next_char
      g_utf8_find_prev_char
      g_utf8_prev_char
    */
    rb_define_module_function(mGLib, "utf8_size", rbglib_m_utf8_strlen, 1);
    /*
      Not implemented.
      g_utf8_strncpy
      g_utf8_strrchr
      g_utf8_strreverse
    */
    rb_define_module_function(mGLib, "utf8_validate",
                              rbglib_m_utf8_validate, 1);

    rb_define_module_function(mGLib, "utf8_upcase",
                              rbglib_m_utf8_strup, 1);
    rb_define_module_function(mGLib, "utf8_downcase",
                              rbglib_m_utf8_strdown, 1);
    rb_define_module_function(mGLib, "utf8_casefold",
                              rbglib_m_utf8_casefold, 1);

    rb_define_module_function(mGLib, "utf8_normalize",
                              rbglib_m_utf8_normalize, -1);

    /*
      ToDo:
      g_utf8_collate_key
      g_utf8_collate_key_for_filename
      g_utf8_to_utf16
      g_utf8_to_ucs4
      g_utf8_to_ucs4_fast
      g_utf16_to_ucs4
      g_utf16_to_utf8
      g_ucs4_to_utf16
      g_ucs4_to_utf8
      g_unichar_to_utf8
    */
}
