/*
 *  Copyright (C) 2015  Ruby-GNOME2 Project Team
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

#ifdef G_OS_WIN32
#include <windows.h>
#endif

#define RG_TARGET_NAMESPACE cRegex
#define _SELF(s) ((GRegex*)RVAL2BOXED(s, G_TYPE_REGEX))
  
static VALUE
rg_initialize(gint argc, VALUE *argv, VALUE self)
{
    VALUE pattern, compile_options, match_options;
    GError *error = NULL;
    GRegex *regex = NULL;

    rb_scan_args(argc, argv, "30", &pattern, &compile_options, &match_options);

    regex = g_regex_new(RVAL2CSTR(pattern), 
                        NUM2UINT(compile_options),
                        NUM2UINT(match_options),
                        &error);
    
    if(error)
      RAISE_GERROR(error);

    G_INITIALIZE(self, regex); 
    return Qnil;
}

static VALUE
rg_match_flags(VALUE self)
{
  return UINT2NUM(g_regex_get_match_flags(_SELF(self)));
}

static VALUE
rg_compile_flags(VALUE self)
{
  return UINT2NUM(g_regex_get_compile_flags(_SELF(self)));
}

static VALUE
rg_pattern(VALUE self)
{
  return CSTR2RVAL(g_regex_get_pattern(_SELF(self)));
}

static VALUE
rg_max_backref(VALUE self)
{
  return INT2NUM(g_regex_get_max_backref(_SELF(self)));
}

static VALUE
rg_s_match_simple(gint argc, VALUE *argv, VALUE self)
{
    VALUE pattern, string, compile_options, match_options;
    gboolean matched;
    
    rb_scan_args(argc, argv, "40", &pattern, &string, &compile_options, &match_options);
    matched = g_regex_match_simple(RVAL2CSTR(pattern), 
                                   RVAL2CSTR(string),
                                   NUM2UINT(compile_options),
                                   NUM2UINT(match_options));
    if(matched == TRUE)
      return Qtrue;
    else
      return Qfalse;
}
void
Init_glib_regex(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_REGEX, "Regex", mGLib); 

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(pattern, 0);
    RG_DEF_METHOD(compile_flags, 0);
    RG_DEF_METHOD(match_flags, 0);
    RG_DEF_METHOD(max_backref, 0);

    RG_DEF_SMETHOD(match_simple, -1);

    G_DEF_CLASS(G_TYPE_REGEX_MATCH_FLAGS, "RegexMatchFlags", mGLib);
    G_DEF_CLASS(G_TYPE_REGEX_COMPILE_FLAGS, "RegexCompileFlags", mGLib);
}
