/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangolanguage.c -

  $Author: mutoh $
  $Date: 2003/01/10 19:22:13 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoLanguage*)RVAL2BOXED(self, PANGO_TYPE_LANGUAGE))


static VALUE
language_initialize(self, language)
    VALUE self, language;
{
    G_INITIALIZE(self, pango_language_from_string(RVAL2CSTR(language)));
    return Qnil;
}

static VALUE
language_s_to_string(self, language)
    VALUE self, language;
{
    return CSTR2RVAL(pango_language_to_string(RVAL2CSTR(language)));
}

static VALUE
language_matches(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE range_list;

    rb_scan_args(argc, argv, "01", &range_list);

    return CBOOL2RVAL(pango_language_matches(_SELF(self), 
                                             NIL_P(range_list) ? NULL : RVAL2CSTR(range_list)));
}

static VALUE
language_to_str(self)
    VALUE self;
{
    return CSTR2RVAL((char*)_SELF(self));
}

void
Init_pango_language()
{
    VALUE pLanguage = G_DEF_CLASS(PANGO_TYPE_LANGUAGE, "Language", mPango);
    
    rb_define_method(pLanguage, "initialize", language_initialize, 1);
    rb_define_singleton_method(pLanguage, "to_string", language_s_to_string, 1);
    rb_define_method(pLanguage, "matches", language_matches, -1);
    rb_define_method(pLanguage, "to_str", language_to_str, 0);
}
