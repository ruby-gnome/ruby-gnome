/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontmap.c -

  $Author: mutoh $
  $Date: 2005/09/17 17:09:13 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_FONT_MAP(RVAL2GOBJ(self)))
#define RVAL2DESC(d) ((PangoFontDescription*)RVAL2BOXED(d, PANGO_TYPE_FONT_DESCRIPTION))
#define RVAL2LANG(l) ((PangoLanguage*)RVAL2BOXED(l, PANGO_TYPE_LANGUAGE))

/*
static VALUE
font_map_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, pango_ft2_font_map_new());
    return Qnil;
}
*/

static VALUE
font_map_load_font(self, context, desc)
    VALUE self, context, desc;
{
    return GOBJ2RVAL(pango_font_map_load_font(_SELF(self), 
                                              PANGO_CONTEXT(RVAL2GOBJ(context)),
                                              RVAL2DESC(desc)));
}

static VALUE
font_map_load_fontset(self, context, desc, lang)
    VALUE self, context, desc, lang;
{
    return GOBJ2RVAL(pango_font_map_load_fontset(_SELF(self),
                                                 PANGO_CONTEXT(RVAL2GOBJ(context)),
                                                 RVAL2DESC(desc),
                                                 RVAL2LANG(lang)));
}

static VALUE
font_map_list_families(self)
    VALUE self;
{
    int n_families;
    PangoFontFamily** families;
    int i;
    VALUE result;

    pango_font_map_list_families(_SELF(self),
                                 &families,
                                 &n_families);

    result = rb_ary_new2(n_families);
    for (i = 0; i < n_families; i++)
      rb_ary_store(result, i, GOBJ2RVAL(families[i]));

    g_free(families);

    return result;
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
font_map_get_shape_engine_type(self)
    VALUE self;
{
    return CSTR2RVAL(pango_font_map_get_shape_engine_type(_SELF(self)));
}
#endif

void
Init_pango_font_map()
{
    VALUE pMap = G_DEF_CLASS(PANGO_TYPE_FONT_MAP, "FontMap", mPango);
    
	 /*
    rb_define_method(pMap, "initialize", font_map_initialize, 0);
	 */
    rb_define_method(pMap, "load_font", font_map_load_font, 2);
    rb_define_method(pMap, "load_fontset", font_map_load_fontset, 3);
    rb_define_method(pMap, "families", font_map_list_families, 0);
#if PANGO_CHECK_VERSION(1,4,0)
    rb_define_method(pMap, "shape_engine_type", font_map_get_shape_engine_type, 0);
#endif

    G_DEF_CLASS3("PangoFcFontMap", "FcFontMap", mPango);
    G_DEF_CLASS3("PangoXftFontMap", "XftFontMap", mPango);
    G_DEF_CLASS3("PangoFT2FontMap", "FT2FontMap", mPango);
    G_DEF_CLASS3("PangoXFontMap", "XFontMap", mPango);
    G_DEF_CLASS3("PangoWin32FontMap", "Win32FontMap", mPango);
    G_DEF_CLASS3("PangoCairoFontMap", "CairoFontMap", mPango);
    G_DEF_CLASS3("PangoCairoFcFontMap", "CairoFcFontMap", mPango);
    G_DEF_CLASS3("PangoCairoWin32FontMap", "CairoWin32FontMap", mPango);
}

