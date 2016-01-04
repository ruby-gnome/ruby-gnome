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

static void
regex_free(gpointer object)
{
    GRegex *regex = object;

    g_regex_unref(regex);
}

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
rg_capture_count(VALUE self)
{
  return INT2NUM(g_regex_get_capture_count(_SELF(self)));
}

static VALUE
rg_has_cr_or_lf(VALUE self)
{
  gboolean contains_cr_or_lf = g_regex_get_has_cr_or_lf(_SELF(self));
  
  if(contains_cr_or_lf == TRUE)
    return Qtrue;
  else
    return Qfalse;
}

static VALUE
rg_max_lookbehind(VALUE self)
{
  return INT2NUM(g_regex_get_max_lookbehind(_SELF(self)));
}

static VALUE
rg_string_number(gint argc, VALUE *argv, VALUE self)
{
  VALUE string;
  rb_scan_args(argc, argv, "10", &string);
  
  return INT2NUM(g_regex_get_string_number(_SELF(self), RVAL2CSTR(string)));
}

static VALUE
rg_split(gint argc, VALUE *argv, VALUE self)
{
  VALUE string, match_options, array_of_strings;
  rb_scan_args(argc, argv, "20", &string, &match_options);
  gchar **strings;

  strings = g_regex_split(_SELF(self),
                          RVAL2CSTR(string),
                          NUM2UINT(match_options));
  
  array_of_strings = rb_ary_new();
  gchar **ptr;
  for(ptr = strings; *ptr != NULL; ptr++)
  {
    rb_ary_push(array_of_strings, CSTR2RVAL(*ptr));
  }
  
  g_strfreev(strings);
  return array_of_strings;
}

static VALUE
rg_split_full(gint argc, VALUE *argv, VALUE self)
{
  VALUE string, start_position, match_options, max_tokens, array_of_strings;
  GError *error = NULL;
  rb_scan_args(argc, argv, "40", &string, &start_position, &match_options, &max_tokens);
  gchar **strings;

  strings = g_regex_split_full(_SELF(self),
                               RVAL2CSTR(string),
                               -1,
                               NUM2INT(start_position),
                               NUM2UINT(match_options),
                               NUM2INT(max_tokens),
                               &error);
  if(error)
    RAISE_GERROR(error);

  array_of_strings = rb_ary_new();
  gchar **ptr;
  for(ptr = strings; *ptr != NULL; ptr++)
  {
    rb_ary_push(array_of_strings, CSTR2RVAL(*ptr));
  }
  
  g_strfreev(strings);
  return array_of_strings;
}

static VALUE
rg_replace(gint argc, VALUE *argv, VALUE self)
{
  VALUE string, start_position, replacement, match_options;
  GError *error = NULL;
  rb_scan_args(argc, argv, "40", &string, &start_position, &replacement, &match_options);
  gchar *modified_string;

  modified_string = g_regex_replace(_SELF(self),
                                    RVAL2CSTR(string),
                                    -1,
                                    NUM2INT(start_position),
                                    RVAL2CSTR(replacement),
                                    NUM2UINT(match_options),
                                    &error);
  if(error)
    RAISE_GERROR(error);
 
  return CSTR2RVAL(modified_string);
}
  
static VALUE
rg_replace_literal(gint argc, VALUE *argv, VALUE self)
{
  VALUE string, start_position, replacement, match_options;
  GError *error = NULL;
  rb_scan_args(argc, argv, "40", &string, &start_position, &replacement, &match_options);
  gchar *modified_string;

  modified_string = g_regex_replace_literal(_SELF(self),
                                    RVAL2CSTR(string),
                                    -1,
                                    NUM2INT(start_position),
                                    RVAL2CSTR(replacement),
                                    NUM2UINT(match_options),
                                    &error);
  if(error)
    RAISE_GERROR(error);
 
  return CSTR2RVAL(modified_string);
}

/* Not to be implemented
 * g_regex_ref
 * g_regex_unref
 * */
static gboolean
g_regex_eval_callback( const GMatchInfo *match_info, GString *result, gpointer user_data)
{
  VALUE cb_match_info, cb_result, callback, returned_data;
  cb_match_info = Qnil;
  cb_match_info = BOXED2RVAL(match_info, G_TYPE_MATCH_INFO);
  cb_result = CSTR2RVAL(result->str);
  callback = (VALUE) user_data;
  gboolean stop_replacement =TRUE;

  returned_data = rb_funcall(callback, rb_intern("call"), 2, cb_match_info, cb_result);
  
  /*  User can return in its callback:
   *  a string, the continue replacement is assumed
   *
   *  an array [string, Qnil] 
   *
   *  any others value are ignored
   *
   * */
  if (TYPE(returned_data) == T_STRING)
  {
    g_string_overwrite(result, 0, RVAL2CSTR(returned_data));
  }
  else
  {  
    if (TYPE(returned_data) == T_ARRAY)
    {
      VALUE string = rb_ary_entry(returned_data, 0);
      if(TYPE(string) == T_STRING)
        g_string_overwrite(result, 0, RVAL2CSTR(string));
      
      if (rb_ary_entry(returned_data, 1) == Qfalse)
        stop_replacement = FALSE;
    }
  }
  
  return stop_replacement;
}
/*
 *  Not sur if this is needed, there must be an easier way (throught ruby)
 * */
static VALUE
rg_replace_eval(gint argc, VALUE *argv, VALUE self)
{
  VALUE string, start_position, match_options;
  rb_scan_args(argc, argv, "30", &string, &start_position, &match_options);
  char *result;
  GError *error = NULL;
  
  if(rb_block_given_p() == 0)
      rb_raise(rb_eTypeError, "Need a block");
  
  VALUE callback = rb_block_proc();
  
  result = g_regex_replace_eval(_SELF(self),
                                RVAL2CSTR(string),
                                -1,
                                NUM2UINT(start_position),
                                NUM2UINT(match_options),
                                g_regex_eval_callback,
                                (gpointer) callback,
                                &error);
  if(error)
    RAISE_GERROR(error);
  
  return CSTR2RVAL(result);
}

/* TODO
 * g_regex_replace_eval
 *
 *
 * implement GLib::MatchInfo first
 * g_regex_match 
 * g_regex_match_full
 * g_regex_match_all
 * g_regex_match_all_full
 * */
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

static VALUE
rg_s_escape_string(gint argc, VALUE *argv, VALUE self)
{
  VALUE string;
  rb_scan_args(argc, argv, "10", &string);

  return CSTR2RVAL(g_regex_escape_string(RVAL2CSTR(string), -1));
}

/*
 * g_regex_escape_null is not implemented because ruby already escape the null caracter 
*/

/* TODO
 * g_regex_split_simple
 * g_regex_check_replacement
 * */
void
Init_glib_regex(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS_WITH_GC_FUNC(G_TYPE_REGEX, "Regex", mGLib,
                                                         NULL, regex_free); 

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(pattern, 0);
    RG_DEF_METHOD(compile_flags, 0);
    RG_DEF_METHOD(match_flags, 0);
    RG_DEF_METHOD(max_backref, 0);
    RG_DEF_METHOD(capture_count, 0);
    RG_DEF_METHOD(has_cr_or_lf, 0);
    RG_DEF_METHOD(max_lookbehind, 0);
    RG_DEF_METHOD(string_number, -1);
    RG_DEF_METHOD(split, -1);
    RG_DEF_METHOD(split_full, -1);
    RG_DEF_METHOD(replace, -1);
    RG_DEF_METHOD(replace_literal, -1);
    RG_DEF_METHOD(replace_eval, -1);

    RG_DEF_SMETHOD(match_simple, -1);
    RG_DEF_SMETHOD(escape_string, -1);

    G_DEF_CLASS(G_TYPE_REGEX_MATCH_FLAGS, "RegexMatchFlags", mGLib);
    G_DEF_CLASS(G_TYPE_REGEX_COMPILE_FLAGS, "RegexCompileFlags", mGLib);
}
