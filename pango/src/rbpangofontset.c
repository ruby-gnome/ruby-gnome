/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontset.c -

  $Author: mutoh $
  $Date: 2005/02/13 17:31:33 $

  Copyright (C) 2002-2005 Masao Mutoh 
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_FONTSET(RVAL2GOBJ(self)))

static ID id_call;

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

#if PANGO_CHECK_VERSION(1,4,0)
static gboolean
fontset_each(fontset, font, func)
    PangoFontset* fontset;
    PangoFont* font;
    gpointer func;
{
    return RTEST(rb_funcall((VALUE)func, id_call, 
                            2, GOBJ2RVAL(fontset), GOBJ2RVAL(font)));
}

static VALUE
fontset_foreach(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    pango_fontset_foreach(_SELF(self), 
                          (PangoFontsetForeachFunc)fontset_each, 
                          (gpointer)func);
    return self;
}
#endif

void
Init_pango_fontset()
{
    VALUE pFontSet = G_DEF_CLASS(PANGO_TYPE_FONTSET, "FontSet", mPango);
    
    id_call = rb_intern("call");

    rb_define_method(pFontSet, "get_font", fontset_get_font, 1);
    rb_define_method(pFontSet, "metrics", fontset_get_metrics, 0);
#if PANGO_CHECK_VERSION(1,4,0)
    rb_define_method(pFontSet, "each", fontset_foreach, 0);
#endif
}
