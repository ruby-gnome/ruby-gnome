/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontset.c -

  $Author: mutoh $
  $Date: 2002/12/31 07:00:58 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_FONTSET(RVAL2GOBJ(self)))

static VALUE
fontset_get_font(self, wc)
    VALUE self, wc;
{
    return GOBJ2RVAL(pango_fontset_get_font(_SELF(self), NUM2UINT(wc)));
}

static VALUE
fontset_get_metrics(self)
    VALUE self;
{
    return BOXED2RVAL(pango_fontset_get_metrics(_SELF(self)), PANGO_TYPE_FONT_METRICS);
}

void
Init_pango_fontset()
{
    VALUE pFontSet = G_DEF_CLASS(PANGO_TYPE_FONTSET, "FontSet", mPango);
    
    rb_define_method(pFontSet, "get_font", fontset_get_font, 1);
    rb_define_method(pFontSet, "metrics", fontset_get_metrics, 0);
}
