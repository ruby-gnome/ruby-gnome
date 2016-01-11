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


#define RG_TARGET_NAMESPACE cRegex
#define _SELF(s) ((GRegex*)RVAL2BOXED(s, G_TYPE_REGEX))

static void
regex_free(gpointer object)
{
    GRegex *regex = object;

    g_regex_unref(regex);
}

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

void
Init_glib_regex(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS_WITH_GC_FUNC(G_TYPE_REGEX, "Regex", mGLib,
                                                         NULL, regex_free); 

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(pattern, 0);
    RG_DEF_METHOD(compile_flags, 0);

    G_DEF_CLASS(G_TYPE_REGEX_MATCH_FLAGS, "RegexMatchFlags", mGLib);
    G_DEF_CLASS(G_TYPE_REGEX_COMPILE_FLAGS, "RegexCompileFlags", mGLib);
}
