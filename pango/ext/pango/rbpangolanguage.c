/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2009  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
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

#include "rbpangoprivate.h"

#define _SELF(self) ((PangoLanguage*)RVAL2BOXED(self, PANGO_TYPE_LANGUAGE))
#define LANG2RVAL(lang) (BOXED2RVAL(lang, PANGO_TYPE_LANGUAGE))

static VALUE
language_s_default(G_GNUC_UNUSED VALUE self)
{
    return LANG2RVAL(pango_language_get_default());
}

static VALUE
language_initialize(VALUE self, VALUE language)
{
    G_INITIALIZE(self, pango_language_from_string(RVAL2CSTR(language)));
    return Qnil;
}

static VALUE
language_s_to_string(G_GNUC_UNUSED VALUE self, VALUE language)
{
    return CSTR2RVAL(pango_language_to_string(RVAL2CSTR(language)));
}

static VALUE
language_matches(int argc, VALUE *argv, VALUE self)
{
    VALUE range_list;

    rb_scan_args(argc, argv, "01", &range_list);

    return CBOOL2RVAL(pango_language_matches(_SELF(self), 
                                             NIL_P(range_list) ? NULL : RVAL2CSTR(range_list)));
}

static VALUE
language_to_str(VALUE self)
{
    return CSTR2RVAL(pango_language_to_string(_SELF(self)));
}

#if PANGO_CHECK_VERSION(1,4,0)
/* Moved from Pango::Script */
static VALUE
language_includes_script(VALUE self, VALUE script)
{
    return CBOOL2RVAL(pango_language_includes_script(_SELF(self), 
                                                     RVAL2GENUM(script, PANGO_TYPE_SCRIPT)));
}
#endif

void
Init_pango_language(void)
{
    VALUE pLanguage = G_DEF_CLASS(PANGO_TYPE_LANGUAGE, "Language", mPango);

    rb_define_singleton_method(pLanguage, "default", language_s_default, 0);

    rb_define_method(pLanguage, "initialize", language_initialize, 1);
    rb_define_singleton_method(pLanguage, "to_string", language_s_to_string, 1);
    rb_define_method(pLanguage, "matches", language_matches, -1);
    rb_define_method(pLanguage, "to_str", language_to_str, 0);
    rb_define_alias(pLanguage, "to_s", "to_str");

#if PANGO_CHECK_VERSION(1,4,0)
    rb_define_method(pLanguage, "includes_script", language_includes_script, 1);
#endif

}
