/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2016  Ruby-GNOME2 Project Team
 *  Copyright (C) 2009  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 KUBO Takehiro
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

#define RG_TARGET_NAMESPACE mGLib

static VALUE
rg_s_convert(G_GNUC_UNUSED VALUE self, VALUE str, VALUE to, VALUE from)
{
    GError *err = NULL;
    gchar* ret;
    gsize written;
    VALUE s = Qnil;

    StringValue(str);
    ret = g_convert(RSTRING_PTR(str), RSTRING_LEN(str),
                    StringValueCStr(to), StringValueCStr(from),
                    NULL, &written, &err);

    if (err != NULL)
        RAISE_GERROR(err);
    {
        rb_encoding *encoding = rb_enc_find(StringValueCStr(to));
        if (!encoding) {
            encoding = rb_ascii8bit_encoding();
        }
        s = rb_enc_str_new(ret, written, encoding);
    }
    g_free(ret);
    return s;
}

static VALUE
rg_s_locale_to_utf8(G_GNUC_UNUSED VALUE self, VALUE str)
{
    GError *err = NULL;
    gchar* ret;
    gsize written;

    StringValue(str);
    ret = g_locale_to_utf8(RSTRING_PTR(str), RSTRING_LEN(str),
                           NULL, &written, &err);

    if (err != NULL)
        RAISE_GERROR(err);
    return CSTR2RVAL_LEN_FREE(ret, written);
}

static VALUE
rg_s_locale_from_utf8(G_GNUC_UNUSED VALUE self, VALUE str)
{
    GError *err = NULL;
    VALUE s = Qnil;
    gchar* ret;
    gsize written;

    StringValue(str);
    ret = g_locale_from_utf8(RSTRING_PTR(str), RSTRING_LEN(str),
                       NULL, &written, &err);

    if (err != NULL)
        RAISE_GERROR(err);
    s = rb_str_new(ret, written);
    g_free(ret);
    return s;
}

static VALUE
rg_s_filename_to_utf8(G_GNUC_UNUSED VALUE self, VALUE str)
{
    GError *err = NULL;
    gchar* ret;
    gsize written;

    StringValue(str);
    ret = g_filename_to_utf8(RSTRING_PTR(str), RSTRING_LEN(str),
                             NULL, &written, &err);

    if (err != NULL)
        RAISE_GERROR(err);
    return CSTR2RVAL_LEN_FREE(ret, written);
}

static VALUE
rg_s_filename_from_utf8(G_GNUC_UNUSED VALUE self, VALUE str)
{
    GError *err = NULL;
    VALUE s = Qnil;
    gchar* ret;
    gsize written;

    StringValue(str);
    ret = g_filename_from_utf8(RSTRING_PTR(str), RSTRING_LEN(str),
                               NULL, &written, &err);

    if (err != NULL)
        RAISE_GERROR(err);
    s = rb_enc_str_new(ret, written, rbg_filename_encoding);
    g_free(ret);
    return s;
}

static VALUE
rg_s_filename_to_uri(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE filename, hostname, s;
    GError *err = NULL;
    gchar* ret;

    rb_scan_args(argc, argv, "11", &filename, &hostname);

    ret = g_filename_to_uri(StringValuePtr(filename),
                            NIL_P(hostname) ? NULL : StringValuePtr(hostname),
                            &err);

    if (err)
        RAISE_GERROR(err);
    s = rb_str_new2(ret);
    g_free(ret);
    return s;
}

static VALUE
rg_s_filename_from_uri(G_GNUC_UNUSED VALUE self, VALUE str)
{
    GError *err = NULL;
    VALUE s;
    gchar* filename;
    char* hostname;

    filename = g_filename_from_uri(StringValuePtr(str), &hostname, &err);

    if (err)
        RAISE_GERROR(err);
    s = rb_ary_new3(2, rb_str_new2(filename),
                    hostname ? rb_str_new2(hostname) : Qnil);
    g_free(filename);
    g_free(hostname);
    return s;
}

static VALUE
rg_s_utf8_validate(G_GNUC_UNUSED VALUE self, VALUE str)
{
    rb_warning("GLib.utf8_validate is deprecated. Use GLib::UTF8.validate instead.");
    StringValue(str);
    return CBOOL2RVAL(g_utf8_validate(RSTRING_PTR(str), RSTRING_LEN(str), NULL));
}

void
Init_glib_convert(void)
{
    VALUE cCharError = G_DEF_ERROR2(G_CONVERT_ERROR, "ConvertError", RG_TARGET_NAMESPACE, rb_eIOError);

    rb_define_const(cCharError, "NO_CONVERSION", INT2NUM(G_CONVERT_ERROR_NO_CONVERSION));
    rb_define_const(cCharError, "ILLEGAL_SEQUENCE", INT2NUM(G_CONVERT_ERROR_ILLEGAL_SEQUENCE));
    rb_define_const(cCharError, "FAILED", INT2NUM(G_CONVERT_ERROR_FAILED));
    rb_define_const(cCharError, "PARTIAL_INPUT", INT2NUM(G_CONVERT_ERROR_PARTIAL_INPUT));
    rb_define_const(cCharError, "BAD_URI", INT2NUM(G_CONVERT_ERROR_BAD_URI));
    rb_define_const(cCharError, "NOT_ABSOLUTE_PATH", INT2NUM(G_CONVERT_ERROR_NOT_ABSOLUTE_PATH));

    /* glib/gunicode.h */
    /* just for backward compatibility.
       Use GLib::UTF8.validate instead. */
    RG_DEF_SMETHOD(utf8_validate, 1);

    /* glib/gconvert.h */
    RG_DEF_SMETHOD(convert, 3);
    RG_DEF_SMETHOD(locale_to_utf8, 1);
    RG_DEF_SMETHOD(locale_from_utf8, 1);
    RG_DEF_SMETHOD(filename_to_utf8, 1);
    RG_DEF_SMETHOD(filename_from_utf8, 1);

    RG_DEF_SMETHOD(filename_to_uri, -1);
    RG_DEF_SMETHOD(filename_from_uri, 1);
}
