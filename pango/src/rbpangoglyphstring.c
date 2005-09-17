/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoglyphstring.c -

  $Author: mutoh $
  $Date: 2005/09/17 17:09:13 $

  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
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

static VALUE
rglyph_extents(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE font, start_index, end_index;
    PangoRectangle ink_rect;
    PangoRectangle logical_rect;

    rb_scan_args(argc, argv, "12", &font, &start_index, &end_index);

    if (NIL_P(start_index)){
        pango_glyph_string_extents(_SELF(self), 
                                   PANGO_FONT(RVAL2GOBJ(font)),
                                   &ink_rect, &logical_rect);
    } else {
        pango_glyph_string_extents_range(_SELF(self), 
                                         NUM2INT(start_index), NUM2INT(end_index),
                                         PANGO_FONT(RVAL2GOBJ(font)),
                                         &ink_rect, &logical_rect);
    }

    return rb_assoc_new(BOXED2RVAL(&ink_rect, PANGO_TYPE_RECTANGLE),
                        BOXED2RVAL(&logical_rect, PANGO_TYPE_RECTANGLE));
}

static VALUE
rglyph_index_to_x(self, text, analysis, index, trailing)
    VALUE self, text, analysis, index, trailing;
{
    int x_pos;
    StringValue(text);

    pango_glyph_string_index_to_x(_SELF(self), RVAL2CSTR(text), 
                                  RSTRING(text)->len, 
                                  (PangoAnalysis*)RVAL2BOXED(analysis, PANGO_TYPE_ANALYSIS),
                                  NUM2INT(index), RTEST(trailing),
                                  &x_pos);
    return INT2NUM(x_pos);
}

static VALUE
rglyph_x_to_index(self, text, analysis, x_pos)
    VALUE self, text, analysis, x_pos;
{
    int index;
    int trailing;

    StringValue(text);
    pango_glyph_string_x_to_index(_SELF(self), RVAL2CSTR(text), RSTRING(text)->len, 
                                  (PangoAnalysis*)RVAL2BOXED(analysis, PANGO_TYPE_ANALYSIS),
                                  NUM2INT(x_pos),
                                  &index, &trailing);
    return rb_assoc_new(INT2NUM(index), CBOOL2RVAL(trailing));
}

static VALUE
rglyph_get_logical_widgths(self, text, embedding_level)
    VALUE self, text, embedding_level;
{
    int* logical_widths = NULL;
    int len, array_len, i;
    char* gtext;
    VALUE ret;
    
    StringValue(text);

    gtext = RVAL2CSTR(text);
    len = RSTRING(text)->len;

    array_len = g_utf8_strlen(gtext, len); 

    logical_widths = g_new(PangoGlyphUnit, array_len);

    pango_glyph_string_get_logical_widths(_SELF(self), gtext, len,
                                          NUM2INT(embedding_level),
                                          logical_widths);

    ret = rb_ary_new();
    for (i = 0; i < array_len; i++){
        rb_ary_push(ret, INT2NUM(logical_widths[i]));
    }

    return ret;
}

static VALUE
rglyph_get_glyphs(self)
    VALUE self;
{
    int i;
    PangoGlyphInfo** glyphs = &_SELF(self)->glyphs;
    gint* log_clusters = _SELF(self)->log_clusters;

    VALUE ret = rb_ary_new();
    for (i = 0; i < _SELF(self)->num_glyphs; i++) {
        rb_ary_push(ret, 
                    rb_assoc_new(BOXED2RVAL(glyphs[i], PANGO_TYPE_GLYPH_INFO),
                                 INT2NUM(log_clusters[i])));
    }
    return ret;
}

void
Init_pango_glyph_string()
{
    VALUE pGlyph = G_DEF_CLASS(PANGO_TYPE_GLYPH_STRING, "GlyphString", mPango);

    rb_define_method(pGlyph, "initialize", rglyph_initialize, 0);
    rb_define_method(pGlyph, "set_size", rglyph_set_size, 1);
    rb_define_method(pGlyph, "extents", rglyph_extents, -1);
    rb_define_method(pGlyph, "index_to_x", rglyph_index_to_x, 4);
    rb_define_method(pGlyph, "x_to_index", rglyph_x_to_index, 3);
    rb_define_method(pGlyph, "get_logical_widths", rglyph_get_logical_widgths, 2);
    rb_define_method(pGlyph, "glyphs", rglyph_get_glyphs, 0);

    G_DEF_SETTERS(pGlyph);
}
