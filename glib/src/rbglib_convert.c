/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib_convert.c -

  $Author: sakai $
  $Date: 2003/03/12 06:41:18 $

  Copyright (C) 2002,2003 KUBO Takehiro

**********************************************************************/
#include <glib.h>
#include "global.h"
#include "rbglib.h"

static VALUE
rbglib_m_charset(self)
    VALUE self;
{
    const char *charset;
    g_get_charset(&charset);
    return rb_str_new2(charset);
}

static VALUE
rbglib_m_convert(self, str, to, from)
    VALUE self, str, to, from;
{
    GError *err = NULL;
    gchar* ret;
    gsize written;
    VALUE s = Qnil;

    StringValue(str);
    ret = g_convert(RSTRING(str)->ptr, RSTRING(str)->len,
                    StringValuePtr(to), StringValuePtr(from),
                    NULL, &written, &err);
    
    if (err != NULL)
        RAISE_GERROR(err);
    s = rb_str_new(ret, written);
    g_free(ret);
    return s;
}

static VALUE
rbglib_m_locale_to_utf8(self, str)
    VALUE self, str;
{
    GError *err = NULL;
    VALUE s = Qnil;
    gchar* ret;
    gsize written;

    StringValue(str);
    ret = g_locale_to_utf8(RSTRING(str)->ptr, RSTRING(str)->len,
                           NULL, &written, &err);

    if (err != NULL)
        RAISE_GERROR(err);
    s = rb_str_new(ret, written);
    g_free(ret);
    return s;
}

static VALUE
rbglib_m_locale_from_utf8(self, str)
    VALUE self, str;
{
    GError *err = NULL;
    VALUE s = Qnil;
    gchar* ret;
    gsize written;

    StringValue(str);
    ret = g_locale_from_utf8(RSTRING(str)->ptr, RSTRING(str)->len,
                       NULL, &written, &err);

    if (err != NULL)
        RAISE_GERROR(err);
    s = rb_str_new(ret, written);
    g_free(ret);
    return s;
}

static VALUE
rbglib_m_filename_to_utf8(self, str)
    VALUE self, str;
{
    GError *err = NULL;
    VALUE s = Qnil;
    gchar* ret;
    gsize written;

    StringValue(str);
    ret = g_filename_to_utf8(RSTRING(str)->ptr, RSTRING(str)->len,
                             NULL, &written, &err);

    if (err != NULL)
        RAISE_GERROR(err);
    s = rb_str_new(ret, written);
    g_free(ret);
    return s;
}

static VALUE
rbglib_m_filename_from_utf8(self, str)
    VALUE self, str;
{
    GError *err = NULL;
    VALUE s = Qnil;
    gchar* ret;
    gsize written;

    StringValue(str);
    ret = g_filename_from_utf8(RSTRING(str)->ptr, RSTRING(str)->len,
                               NULL, &written, &err);

    if (err != NULL)
        RAISE_GERROR(err);
    s = rb_str_new(ret, written);
    g_free(ret);
    return s;
}

static VALUE
rbglib_m_filename_to_uri(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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
rbglib_m_filename_from_uri(self, str)
    VALUE self, str;
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

void
Init_glib_convert()
{
    /* glib/gunicode.h */
    rb_define_module_function(mGLib, "charset", rbglib_m_charset, 0);

    /* glib/gconvert.h */
    rb_define_module_function(mGLib, "convert", rbglib_m_convert, 3);
    rb_define_module_function(mGLib, "locale_to_utf8", rbglib_m_locale_to_utf8, 1);
    rb_define_module_function(mGLib, "locale_from_utf8", rbglib_m_locale_from_utf8, 1);
    rb_define_module_function(mGLib, "filename_to_utf8", rbglib_m_filename_to_utf8, 1);
    rb_define_module_function(mGLib, "filename_from_utf8", rbglib_m_filename_from_utf8, 1);

    rb_define_module_function(mGLib, "filename_to_uri", rbglib_m_filename_to_uri, -1);
    rb_define_module_function(mGLib, "filename_from_uri", rbglib_m_filename_from_uri, 1);
}
