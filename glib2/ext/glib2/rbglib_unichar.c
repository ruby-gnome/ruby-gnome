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

#define RG_TARGET_NAMESPACE mUniChar

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
DEF_IS_UNICHAR(wide_cjk)

#undef DEF_IS_UNICHAR

static VALUE
rg_s_to_upper(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return UINT2NUM(g_unichar_toupper(NUM2UINT(unichar)));
}

static VALUE
rg_s_to_lower(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return UINT2NUM(g_unichar_tolower(NUM2UINT(unichar)));
}

static VALUE
rg_s_to_title(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return UINT2NUM(g_unichar_totitle(NUM2UINT(unichar)));
}

static VALUE
rg_s_digit_value(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return INT2NUM(g_unichar_digit_value(NUM2UINT(unichar)));
}

static VALUE
rg_s_xdigit_value(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return INT2NUM(g_unichar_xdigit_value(NUM2UINT(unichar)));
}

static VALUE
rg_s_type(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return GENUM2RVAL(g_unichar_type(NUM2UINT(unichar)),
                      G_TYPE_UNICODE_TYPE);
}

static VALUE
rg_s_break_type(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return GENUM2RVAL(g_unichar_break_type(NUM2UINT(unichar)),
                      G_TYPE_UNICODE_BREAK_TYPE);
}

static VALUE
rg_s_get_mirror_char(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    gunichar mirrored_char;

    if (g_unichar_get_mirror_char(NUM2UINT(unichar), &mirrored_char)) {
        return UINT2NUM(mirrored_char);
    } else {
        return unichar;
    }
}

static VALUE
rg_s_combining_class(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return INT2NUM(g_unichar_combining_class(NUM2UINT(unichar)));
}

static VALUE
rg_s_get_script(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return GENUM2RVAL(g_unichar_get_script(NUM2UINT(unichar)),
                      G_TYPE_UNICODE_SCRIPT);
}

static VALUE
rg_s_mark_p(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return CBOOL2RVAL(g_unichar_ismark(NUM2UINT(unichar)));
}

static VALUE
rg_s_zero_width_p(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    return CBOOL2RVAL(g_unichar_iszerowidth(NUM2UINT(unichar)));
}

static VALUE
rg_s_to_utf8(G_GNUC_UNUSED VALUE self, VALUE unichar)
{
    gchar utf8[6];
    gint len;

    len = g_unichar_to_utf8(NUM2UINT(unichar), utf8);
    return CSTR2RVAL_LEN(utf8, len);
}

static VALUE
rg_s_compose(G_GNUC_UNUSED VALUE self, VALUE unichar1, VALUE unichar2)
{
    gunichar composed_char;
    if (g_unichar_compose(NUM2UINT(unichar1),
                          NUM2UINT(unichar2),
                          &composed_char)) {
        return UINT2NUM(composed_char);
    } else {
        return Qnil;
    }
}

static VALUE
rg_s_decompose(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE unichar;
    VALUE options = Qnil;
    gboolean compat = FALSE;

    rb_scan_args(argc, argv, "1:", &unichar, &options);
    if (!NIL_P(options)) {
        ID keywords[1];
        VALUE values[1];
        keywords[0] = rb_intern("compat");
        rb_get_kwargs(options, keywords, 0, 1, values);
        if (values[0] != Qundef) {
            compat = CBOOL2RVAL(values[0]);
        }
    }

    /* 18 is enough. See g_unichar_fully_decompose() document. */
    gunichar result[G_UNICHAR_MAX_DECOMPOSITION_LENGTH];
    gsize result_len =
        g_unichar_fully_decompose(NUM2UINT(unichar),
                                  compat,
                                  result,
                                  G_UNICHAR_MAX_DECOMPOSITION_LENGTH);
    return CSTR2RVAL_LEN_UCS4((const char *)result,
                              result_len * sizeof(gunichar));
}

void
Init_glib_unichar(void)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGLib, "UniChar");

    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "alnum?",
                              rbglib_m_unichar_isalnum, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "alpha?",
                              rbglib_m_unichar_isalpha, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "cntrl?",
                              rbglib_m_unichar_iscntrl, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "digit?",
                              rbglib_m_unichar_isdigit, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "graph?",
                              rbglib_m_unichar_isgraph, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "lower?",
                              rbglib_m_unichar_islower, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "print?",
                              rbglib_m_unichar_isprint, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "punct?",
                              rbglib_m_unichar_ispunct, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "space?",
                              rbglib_m_unichar_isspace, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "upper?",
                              rbglib_m_unichar_isupper, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "xdigit?",
                              rbglib_m_unichar_isxdigit, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "title?",
                              rbglib_m_unichar_istitle, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "defined?",
                              rbglib_m_unichar_isdefined, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "wide?",
                              rbglib_m_unichar_iswide, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "wide_cjk?",
                              rbglib_m_unichar_iswide_cjk, 1);

    RG_DEF_SMETHOD(to_upper, 1);
    RG_DEF_SMETHOD(to_lower, 1);
    RG_DEF_SMETHOD(to_title, 1);

    RG_DEF_SMETHOD(digit_value, 1);
    RG_DEF_SMETHOD(xdigit_value, 1);

    RG_DEF_SMETHOD(type, 1);
    RG_DEF_SMETHOD(break_type, 1);

    RG_DEF_SMETHOD(get_mirror_char, 1);

    RG_DEF_SMETHOD(combining_class, 1);
    RG_DEF_SMETHOD(get_script, 1);
    RG_DEF_SMETHOD_P(mark, 1);
    RG_DEF_SMETHOD_P(zero_width, 1);

    RG_DEF_SMETHOD(to_utf8, 1);

    RG_DEF_SMETHOD(compose, 2);
    RG_DEF_SMETHOD(decompose, -1);
}
