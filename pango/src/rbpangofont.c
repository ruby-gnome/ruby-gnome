/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofont.c -

  $Author: mutoh $
  $Date: 2002/12/31 07:00:57 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_FONT(RVAL2GOBJ(self)))

/*
PangoEngineShape* pango_font_find_shaper    (PangoFont *font,
                                             PangoLanguage *language,
                                             guint32 ch);
*/

static VALUE
font_describe(self)
    VALUE self;
{
    return BOXED2RVAL(pango_font_describe(_SELF(self)), PANGO_TYPE_FONT_DESCRIPTION);
}

/*
PangoCoverage* pango_font_get_coverage      (PangoFont *font,
                                             PangoLanguage *language);
void        pango_font_get_glyph_extents    (PangoFont *font,
                                             PangoGlyph glyph,
                                             PangoRectangle *ink_rect,
                                             PangoRectangle *logical_rect);
*/

static VALUE
font_get_metrics(self, language)
    VALUE self, language;
{
    PangoLanguage* lang = (PangoLanguage*)RVAL2BOXED(language, PANGO_TYPE_LANGUAGE);
    return BOXED2RVAL(pango_font_get_metrics(_SELF(self), lang), 
                      PANGO_TYPE_FONT_DESCRIPTION);
}
void
Init_pango_font()
{
    VALUE pFont = G_DEF_CLASS(PANGO_TYPE_FONT, "Font", mPango);
    
    rb_define_method(pFont, "describe", font_describe, 0);
    rb_define_method(pFont, "metrics", font_get_metrics, 0);
}
