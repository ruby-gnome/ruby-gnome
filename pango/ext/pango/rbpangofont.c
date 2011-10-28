/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbpangoprivate.h"

#define RG_TARGET_NAMESPACE cFont
#define _SELF(self) (PANGO_FONT(RVAL2GOBJ(self)))

static VALUE
rg_find_shaper(VALUE self, VALUE language, VALUE ch)
{
    return GOBJ2RVAL(pango_font_find_shaper(_SELF(self), 
                                            (PangoLanguage*)RVAL2BOXED(language, PANGO_TYPE_LANGUAGE),
                                            NUM2UINT(ch)));
}

static VALUE
rg_describe(int argc, VALUE *argv, VALUE self)
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
rg_get_coverage(VALUE self, VALUE language)
{
    PangoCoverage* c = pango_font_get_coverage(_SELF(self), 
                                               (PangoLanguage*)RVAL2BOXED(language, PANGO_TYPE_LANGUAGE));
    return BOXED2RVAL(c, PANGO_TYPE_COVERAGE);
}

static VALUE
rg_get_glyph_extents(VALUE self, VALUE glyph)
{
    PangoRectangle ink_rect, logical_rect;
    pango_font_get_glyph_extents(_SELF(self),
                                 NUM2UINT(glyph),
                                 &ink_rect, &logical_rect);

    return rb_assoc_new(BOXED2RVAL(&ink_rect, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&logical_rect, PANGO_TYPE_RECTANGLE));

}

static VALUE
rg_metrics(int argc, VALUE *argv, VALUE self)
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
rg_font_map(VALUE self)
{
    return GOBJ2RVAL(pango_font_get_font_map(_SELF(self)));
}
#endif

void
Init_pango_font(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_FONT, "Font", mPango);

    RG_DEF_METHOD(find_shaper, 2);
    RG_DEF_METHOD(describe, -1);
    RG_DEF_METHOD(get_coverage, 1);
    RG_DEF_METHOD(get_glyph_extents, 1);
    RG_DEF_METHOD(metrics, -1);

#if PANGO_CHECK_VERSION(1,9,0)
    RG_DEF_METHOD(font_map, 0);
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
