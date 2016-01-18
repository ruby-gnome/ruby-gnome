/*
 *  Copyright (C) 2015-2016  Ruby-GNOME2 Project Team
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


#define RG_TARGET_NAMESPACE cRegex
#define _SELF(s) ((GRegex*)RVAL2BOXED(s, G_TYPE_REGEX))

static VALUE
rg_initialize(gint argc, VALUE *argv, VALUE self)
{
    GError *error = NULL;
    GRegex *regex = NULL;

    VALUE rb_pattern, rb_compile_options, rb_match_options;
    VALUE rb_options;
    const char *pattern;
    GRegexCompileFlags compile_options = 0;
    GRegexMatchFlags match_options = 0;

    rb_scan_args(argc, argv, "11", &rb_pattern, &rb_options);
    rbg_scan_options(rb_options,
                     "compile_options", &rb_compile_options,
                     "match_options", &rb_match_options,
                     NULL);

    pattern = RVAL2CSTR(rb_pattern);
    if (!NIL_P(rb_compile_options))
        compile_options = RVAL2GREGEXCOMPILEOPTIONSFLAGS(rb_compile_options);
    if (!NIL_P(rb_match_options))
        match_options = RVAL2GREGEXMATCHOPTIONSFLAGS(rb_match_options);

    regex = g_regex_new(pattern,
                        compile_options,
                        match_options,
                        &error);
    if (error)
        RAISE_GERROR(error);

    G_INITIALIZE(self, regex);
    return Qnil;
}

static VALUE
rg_pattern(VALUE self)
{
    return CSTR2RVAL(g_regex_get_pattern(_SELF(self)));
}

static VALUE
rg_compile_flags(VALUE self)
{
    return UINT2NUM(g_regex_get_compile_flags(_SELF(self)));
}

static VALUE
rg_match_flags(VALUE self)
{
    return UINT2NUM(g_regex_get_match_flags(_SELF(self)));
}

static VALUE
rg_split(gint argc, VALUE *argv, VALUE self)
{
    VALUE rb_string, rb_start_position, rb_match_options, rb_max_tokens, rb_options;
    GError *error = NULL;
    gchar **strings;
    const gchar *string;
    gssize string_len = -1;
    gint start_position = 0;
    GRegexMatchFlags match_options = 0;
    gint max_tokens = 0;

    rb_scan_args(argc, argv, "11", &rb_string, &rb_options);

    rbg_scan_options(rb_options,
                     "start_position", &rb_start_position,
                     "match_options", &rb_match_options,
                     "max_tokens", &rb_max_tokens,
                     NULL);
    string = RVAL2CSTR(rb_string);
    string_len = RSTRING_LEN(rb_string);

    if (!NIL_P(rb_start_position))
        start_position = NUM2INT(rb_start_position);
    if (!NIL_P(rb_match_options))
        match_options = RVAL2GREGEXMATCHOPTIONSFLAGS(rb_match_options);
    if (!NIL_P(rb_max_tokens))
        max_tokens = NUM2INT(rb_max_tokens);

    strings = g_regex_split_full(_SELF(self),
                                 string,
                                 string_len,
                                 start_position,
                                 match_options,
                                 max_tokens,
                                 &error);

    if (error)
      RAISE_GERROR(error);

    return STRV2RVAL_FREE(strings);
}

static VALUE
rg_match(gint argc, VALUE *argv, VALUE self)
{
    VALUE rb_string, rb_start_position, rb_match_options, 
          rb_options, dup_string, match_info;
    GMatchInfo *_match_info = NULL;
    GError *error = NULL;
    const gchar *string;
    gssize string_len = -1;
    gint start_position = 0;
    GRegexMatchFlags match_options = 0;

    rb_scan_args(argc, argv, "11", &rb_string, &rb_options);

    rbg_scan_options(rb_options,
                     "start_position", &rb_start_position,
                     "match_options", &rb_match_options,
                     NULL);
    
    dup_string = rb_str_dup(rb_string);
    rb_str_freeze(dup_string);

    string = RVAL2CSTR(dup_string);
    string_len = RSTRING_LEN(dup_string);


    if (!NIL_P(rb_start_position))
        start_position = NUM2INT(rb_start_position);
    if (!NIL_P(rb_match_options))
        match_options = RVAL2GREGEXMATCHOPTIONSFLAGS(rb_match_options);

    g_regex_match_full(_SELF(self),
                       string,
                       string_len  ,
                       start_position,
                       match_options,
                       &_match_info,
                       &error);
  
    if (error)
        RAISE_GERROR(error);

    if (_match_info)
    {
        match_info = BOXED2RVAL(_match_info, G_TYPE_MATCH_INFO);
        rb_iv_set(match_info, "@string", dup_string);
        return match_info;
    }
    else
        return Qnil;
}

  void
Init_glib_regex(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_REGEX, "Regex", mGLib);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(pattern, 0);
    RG_DEF_METHOD(compile_flags, 0);
    RG_DEF_METHOD(match_flags, 0);
    RG_DEF_METHOD(split, -1);
    RG_DEF_METHOD(match, -1);

    G_DEF_CLASS(G_TYPE_REGEX_MATCH_FLAGS, "RegexMatchFlags", mGLib);
    G_DEF_CLASS(G_TYPE_REGEX_COMPILE_FLAGS, "RegexCompileFlags", mGLib);
}
