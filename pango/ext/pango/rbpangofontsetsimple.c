/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontsetsimple.c -

  $Author: mutoh $
  $Date: 2005/10/14 09:55:57 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "rbpango.h"

#ifdef PANGO_ENABLE_BACKEND

#define _SELF(self) (PANGO_FONTSET_SIMPLE(RVAL2GOBJ(self)))
#define RVAL2LANG(v) ((PangoLanguage*)RVAL2BOXED(v, PANGO_TYPE_LANGUAGE))

static VALUE
fontset_simple_initialize(VALUE self, VALUE lang)
{
    G_INITIALIZE(self, pango_fontset_simple_new(RVAL2LANG(lang)));
    return Qnil;
}

static VALUE
fontset_simple_append(VALUE self, VALUE font)
{
    pango_fontset_simple_append(_SELF(self), PANGO_FONT(RVAL2GOBJ(font)));
    return self;
}

static VALUE
fontset_simple_size(VALUE self)
{
    return INT2NUM(pango_fontset_simple_size(_SELF(self)));
}

#endif
void
Init_pango_fontset_simple()
{
#ifdef PANGO_ENABLE_BACKEND
    VALUE pFSimple = G_DEF_CLASS(PANGO_TYPE_FONTSET_SIMPLE, "FontsetSimple", mPango);
    
    rb_define_method(pFSimple, "initialize", fontset_simple_initialize, 1);
    rb_define_method(pFSimple, "append", fontset_simple_append, 1);
    rb_define_method(pFSimple, "size", fontset_simple_size, 0);
#endif
}
