/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofont.c -

  $Author: mutoh $
  $Date: 2003/08/17 07:59:22 $

  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
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
font_get_metrics(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE language;
    PangoLanguage* lang = NULL;
    
    rb_scan_args(argc, argv, "01", &language);
    
    if (!NIL_P(language))
        lang = (PangoLanguage*)RVAL2BOXED(language, PANGO_TYPE_LANGUAGE);

    return BOXED2RVAL(pango_font_get_metrics(_SELF(self), lang), 
                      PANGO_TYPE_FONT_METRICS);
}

void
Init_pango_font()
{
    VALUE pFont = G_DEF_CLASS(PANGO_TYPE_FONT, "Font", mPango);
    
    rb_define_method(pFont, "describe", font_describe, 0);
    rb_define_method(pFont, "metrics", font_get_metrics, -1);
}
