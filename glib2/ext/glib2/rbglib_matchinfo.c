/*
 *  Copyright (C) 2015-2021  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE cMatchInfo
#define _SELF(s) ((GMatchInfo*)RVAL2BOXED(s, G_TYPE_MATCH_INFO))

static VALUE
rg_regex(VALUE self)
{
    GRegex *regex;
    regex = g_match_info_get_regex(_SELF(self));
    return BOXED2RVAL(regex, G_TYPE_REGEX);
}

static VALUE
rg_string(VALUE self)
{
    return rb_iv_get(self, "@string");
}

static VALUE
rg_matches_p(VALUE self)
{
    return CBOOL2RVAL(g_match_info_matches(_SELF(self)));
}

static VALUE
rg_match_count(VALUE self)
{
    return INT2NUM(g_match_info_get_match_count(_SELF(self)));
}

static VALUE
rg_partial_match_p(VALUE self)
{
    return CBOOL2RVAL(g_match_info_is_partial_match(_SELF(self)));
}

static VALUE
rg_fetch(VALUE self, VALUE rb_match_reference)
{
    gchar *match;

    switch (TYPE(rb_match_reference)) {
      case RUBY_T_FIXNUM:
        {
            gint match_num;
            match_num = NUM2INT(rb_match_reference);
            match = g_match_info_fetch(_SELF(self), match_num);
        }
        break;
      case RUBY_T_STRING:
      case RUBY_T_SYMBOL:
        {
            const gchar *match_name;
            match_name = RVAL2CSTR_ACCEPT_SYMBOL(rb_match_reference);
            match = g_match_info_fetch_named(_SELF(self), match_name);
        }
        break;
      default:
        rb_raise(rb_eArgError, "Expected a String, a Symbol or an Integer");
        break;
    }

    return CSTR2RVAL_FREE(match);
}

static VALUE
rg_fetch_pos(VALUE self, VALUE rb_match_reference)
{
    gint start_pos = 0;
    gint end_pos = 0;
    gboolean fetched = FALSE;

    switch (TYPE(rb_match_reference)) {
      case RUBY_T_FIXNUM:
        {
            gint match_num;
            match_num = NUM2INT(rb_match_reference);
            fetched = g_match_info_fetch_pos(_SELF(self), match_num,
                                             &start_pos, &end_pos);
        }
        break;
      case RUBY_T_STRING:
      case RUBY_T_SYMBOL:
        {
            const gchar *match_name;
            match_name = RVAL2CSTR_ACCEPT_SYMBOL(rb_match_reference);
            fetched = g_match_info_fetch_named_pos(_SELF(self), match_name,
                                                   &start_pos, &end_pos);
        }
        break;
      default:
        rb_raise(rb_eArgError, "Expected a String, a Symbol or an Integer");
        break;
    }

    if (!fetched) {
        return Qnil;
    }

    return rb_ary_new_from_args(2, INT2NUM(start_pos), INT2NUM(end_pos));
}

static VALUE
rg_fetch_all(VALUE self)
{
    gchar **strings;
    strings = g_match_info_fetch_all(_SELF(self));
    return STRV2RVAL_FREE(strings);
}

static VALUE
rg_next(VALUE self)
{
    gboolean matched;
    GError *error = NULL;

    matched = g_match_info_next(_SELF(self), &error);

    if (error)
        RAISE_GERROR(error);

    return CBOOL2RVAL(matched);
}

static VALUE
rg_expand_references(VALUE self, VALUE rb_string)
{
    const gchar *string = RVAL2CSTR(rb_string);
    gchar *expanded_string = NULL;
    GError *error = NULL;

    expanded_string = g_match_info_expand_references(_SELF(self),
                                                     string,
                                                     &error);
    if (error)
        RAISE_GERROR(error);

    return CSTR2RVAL_FREE(expanded_string);
}

void
Init_glib_matchinfo(void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_MATCH_INFO, "MatchInfo", mGLib);
    RG_DEF_METHOD(regex, 0);
    RG_DEF_METHOD(string, 0);
    RG_DEF_METHOD_P(matches, 0);
    RG_DEF_METHOD(match_count, 0);
    RG_DEF_METHOD_P(partial_match, 0);
    RG_DEF_METHOD(fetch, 1);
    RG_DEF_ALIAS("[]", "fetch");
    RG_DEF_METHOD(fetch_pos, 1);
    RG_DEF_ALIAS("fetch_position", "fetch_pos");
    RG_DEF_METHOD(fetch_all, 0);
    RG_DEF_METHOD(next, 0);
    RG_DEF_METHOD(expand_references, 1);
}
