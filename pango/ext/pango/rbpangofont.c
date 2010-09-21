/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofont.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:33 $

  Copyright (C) 2002-2006 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_FONT(RVAL2GOBJ(self)))

static VALUE
font_find_shaper(self, language, ch)
    VALUE self, language, ch;
{
    return GOBJ2RVAL(pango_font_find_shaper(_SELF(self), 
                                            (PangoLanguage*)RVAL2BOXED(language, PANGO_TYPE_LANGUAGE),
                                            NUM2UINT(ch)));
}

static VALUE
font_describe(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE desc, absolute_size;
    rb_scan_args(argc, argv, "01", &absolute_size);

    if (NIL_P(absolute_size) || ! RVAL2CBOOL(absolute_size)){
        desc = BOXED2RVAL(pango_font_describe(_SELF(self)), PANGO_TYPE_FONT_DESCRIPTION);
    } else {
#if PANGO_CHECK_VERSION(1,14,0)
        desc = BOXED2RVAL(pango_font_describe_with_absolute_size(_SELF(self)), 
                          PANGO_TYPE_FONT_DESCRIPTION);
#else
        rb_warning("Pango::Font#describe(absolute) has been supported since GTK+-2.10.x. Use Pango::Font#describe() instead."); 
        desc = BOXED2RVAL(pango_font_describe(_SELF(self)), PANGO_TYPE_FONT_DESCRIPTION);
#endif
    }
    return desc;
}

static VALUE
font_get_coverage(self, language)
    VALUE self, language;
{
    PangoCoverage* c = pango_font_get_coverage(_SELF(self), 
                                               (PangoLanguage*)RVAL2BOXED(language, PANGO_TYPE_LANGUAGE));
    return BOXED2RVAL(c, PANGO_TYPE_COVERAGE);
}

static VALUE
font_get_glyph_extents(self, glyph)
    VALUE self, glyph;
{
    PangoRectangle ink_rect, logical_rect;
    pango_font_get_glyph_extents(_SELF(self),
                                 NUM2UINT(glyph),
                                 &ink_rect, &logical_rect);

    return rb_assoc_new(BOXED2RVAL(&ink_rect, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&logical_rect, PANGO_TYPE_RECTANGLE));
                    
}

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

#if PANGO_CHECK_VERSION(1,9,0)
static VALUE
font_get_font_map(self)
    VALUE self;
{
    return GOBJ2RVAL(pango_font_get_font_map(_SELF(self)));
}
#endif

void
Init_pango_font()
{
    VALUE pFont = G_DEF_CLASS(PANGO_TYPE_FONT, "Font", mPango);
    
    rb_define_method(pFont, "find_shaper", font_find_shaper, 2);
    rb_define_method(pFont, "describe", font_describe, -1);
    rb_define_method(pFont, "get_coverage", font_get_coverage, 1);
    rb_define_method(pFont, "get_glyph_extents", font_get_glyph_extents, 1);
    rb_define_method(pFont, "metrics", font_get_metrics, -1);

#if PANGO_CHECK_VERSION(1,9,0)
    rb_define_method(pFont, "font_map", font_get_font_map, 0);
#endif

    G_DEF_CLASS3("PangoXFont", "XFont", mPango);
    G_DEF_CLASS3("PangoFT2Font", "FT2Font", mPango);
    G_DEF_CLASS3("PangoFcFont", "FcFont", mPango);
    G_DEF_CLASS3("PangoXftFont", "XftFont", mPango);
    G_DEF_CLASS3("PangoCairoFcFont", "CairoFcFont", mPango);
    G_DEF_CLASS3("PangoCairoFont", "CairoFont", mPango);
    G_DEF_CLASS3("PangoCairoWin32Font", "CairoWin32Font", mPango);
#if PANGO_CHECK_VERSION(1,12,0)
    G_DEF_CLASS3("PangoATSUIFont", "ATSUIFont", mPango);
#endif
}
