/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontmap.c -

  $Author: mutoh $
  $Date: 2002/12/31 07:00:57 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_FONT_MAP(RVAL2GOBJ(self)))
#define RVAL2DESC(d) ((PangoFontDescription*)RVAL2BOXED(d, PANGO_TYPE_FONT_DESCRIPTION))
#define RVAL2LANG(l) ((PangoLanguage*)RVAL2BOXED(l, PANGO_TYPE_LANGUAGE))

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

/*
void        pango_font_map_list_families    (PangoFontMap *fontmap,
                                             PangoFontFamily ***families,
                                             int *n_families);
*/

void
Init_pango_font_map()
{
    VALUE pFace = G_DEF_CLASS(PANGO_TYPE_FONT_MAP, "FontMap", mPango);
    
    rb_define_method(pFace, "load_font", font_map_load_font, 2);
    rb_define_method(pFace, "load_fontset", font_map_load_fontset, 3);
}
