/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoanalysis.c -

  $Author: mutoh $
  $Date: 2003/11/09 17:10:47 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(s) ((PangoAnalysis*)RVAL2BOXED(s, PANGO_TYPE_ANALYSIS))

/**********************************/
static PangoAnalysis*
ana_copy(const PangoAnalysis* val)
{
    PangoAnalysis* new_val;
    g_return_val_if_fail (val != NULL, NULL);
    new_val = g_new(PangoAnalysis, 1);
    *new_val = *val;
    return new_val;
}
                                                                                     
GType
pango_analysis_get_type(void)
{
    static GType our_type = 0;
                                                                                     
    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoAnalysis",
                    (GBoxedCopyFunc)ana_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/

static VALUE
ana_initialize(self)
    VALUE self;
{
    PangoAnalysis ana = { NULL, NULL, NULL, 0 };
    G_INITIALIZE(self, &ana);
    return Qnil;
}

static VALUE
ana_set_font(self, font)
    VALUE self, font;
{
    _SELF(self)->font = PANGO_FONT(RVAL2GOBJ(font));
    return self;
}

static VALUE
ana_get_font(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->font);
}

static VALUE
ana_set_level(self, level)
    VALUE self, level;
{
    _SELF(self)->level = NUM2UINT(level);
    return self;
}

static VALUE
ana_get_level(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->level);
}

static VALUE
ana_set_language(self, lang)
    VALUE self, lang;
{
    _SELF(self)->language = RVAL2BOXED(lang, PANGO_TYPE_LANGUAGE);
    return self;
}

static VALUE
ana_get_language(self)
    VALUE self;
{
    return BOXED2RVAL(_SELF(self)->language, PANGO_TYPE_LANGUAGE);
}

void
Init_pango_analysis()
{
    VALUE pana = G_DEF_CLASS(PANGO_TYPE_ANALYSIS, "Analysis", mPango);
    
    rb_define_method(pana, "initialize", ana_initialize, 0);
    rb_define_method(pana, "set_font", ana_set_font, 1);
    rb_define_method(pana, "font", ana_get_font, 0);
    rb_define_method(pana, "set_level", ana_set_level, 1);
    rb_define_method(pana, "level", ana_get_level, 0);
    rb_define_method(pana, "set_language", ana_set_language, 1);
    rb_define_method(pana, "language", ana_get_language, 0);

    G_DEF_SETTERS(pana);
}
