/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoglyphstring.c -

  $Author: mutoh $
  $Date: 2002/12/31 07:00:58 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoGlyphString*)(RVAL2BOXED(self, PANGO_TYPE_GLYPH_STRING)))

static VALUE
rglyph_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, pango_glyph_string_new());
    return Qnil;
}

static VALUE
rglyph_set_size(self, len)
    VALUE self, len;
{
    pango_glyph_string_set_size(_SELF(self), NUM2INT(len));
    return self;
}

/*
void        pango_glyph_string_extents      (PangoGlyphString *glyphs,
                                             PangoFont *font,
                                             PangoRectangle *ink_rect,
                                             PangoRectangle *logical_rect);
void        pango_glyph_string_extents_range
                                            (PangoGlyphString *glyphs,
                                             int start,
                                             int end,
                                             PangoFont *font,
                                             PangoRectangle *ink_rect,
                                             PangoRectangle *logical_rect);
void        pango_glyph_string_index_to_x   (PangoGlyphString *glyphs,
                                             char *text,
                                             int length,
                                             PangoAnalysis *analysis,
                                             int index,
                                             gboolean trailing,
                                             int *x_pos);
void        pango_glyph_string_x_to_index   (PangoGlyphString *glyphs,
                                             char *text,
                                             int length,
                                             PangoAnalysis *analysis,
                                             int x_pos,
                                             int *index,
                                             int *trailing);
static VALUE
rglyph_get_logical_widgths(self, text, embedding_level)
    VALUE self, text, embedding_level;
{
    pango_glyph_string_get_logical_widths(_SELF(self), RVAL2CSTR(text),
                                          RSTRING(text)->len, 
                                            (PangoGlyphString *glyphs,
                                             const char *text,
                                             int length,
                                             int embedding_level,
                                             int *logical_widths);

*/
void
Init_pango_glyph_string()
{
    VALUE pGlyph = G_DEF_CLASS(PANGO_TYPE_GLYPH_STRING, "GlyphString", mPango);

    rb_define_method(pGlyph, "initialize", rglyph_initialize, 0);
    rb_define_method(pGlyph, "set_size", rglyph_set_size, 1);

    G_DEF_SETTERS(pGlyph);
}
