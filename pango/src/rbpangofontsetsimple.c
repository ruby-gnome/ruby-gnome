/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontsetsimple.c -

  $Author: mutoh $
  $Date: 2003/02/01 17:13:25 $

  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#ifdef PANGO_ENABLE_BACKEND

#define _SELF(self) (PANGO_FONTSET_SIMPLE(RVAL2GOBJ(self)))
#define RVAL2LANG(v) ((PangoLanguage*)RVAL2BOXED(v, PANGO_TYPE_LANGUAGE))

static VALUE
fontset_simple_initialize(self, lang)
    VALUE self, lang;
{
    G_INITIALIZE(self, pango_fontset_simple_new(RVAL2LANG(lang)));
    return Qnil;
}

static VALUE
fontset_simple_append(self, font)
    VALUE self, font;
{
    pango_fontset_simple_append(_SELF(self), PANGO_FONT(RVAL2GOBJ(font)));
    return self;
}

static VALUE
fontset_simple_size(self)
    VALUE self;
{
    return INT2NUM(pango_fontset_simple_size(_SELF(self)));
}

#endif
void
Init_pango_fontset_simple()
{
#ifdef PANGO_ENABLE_BACKEND
    VALUE pFSimple = G_DEF_CLASS(PANGO_TYPE_FONTSET_SIMPLE, "FontSetSimple", mPango);
    
    rb_define_method(pFSimple, "initialize", fontset_simple_initialize, 1);
    rb_define_method(pFSimple, "append", fontset_simple_append, 1);
    rb_define_method(pFSimple, "size", fontset_simple_size, 0);
#endif
}
