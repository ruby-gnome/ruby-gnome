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

#define RG_TARGET_NAMESPACE cGlyphString
#define _SELF(self) ((PangoGlyphString*)(RVAL2BOXED(self, PANGO_TYPE_GLYPH_STRING)))

static VALUE
rglyph_initialize(VALUE self)
{
    G_INITIALIZE(self, pango_glyph_string_new());
    return Qnil;
}

static VALUE
rglyph_set_size(VALUE self, VALUE len)
{
    pango_glyph_string_set_size(_SELF(self), NUM2INT(len));
    return self;
}

static VALUE
rglyph_extents(int argc, VALUE *argv, VALUE self)
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

#if PANGO_CHECK_VERSION(1,14,0)
static VALUE
rglyph_get_width(VALUE self)
{
    return INT2NUM(pango_glyph_string_get_width(_SELF(self)));
}
#endif

static VALUE
rglyph_index_to_x(VALUE self, VALUE text, VALUE analysis, VALUE index, VALUE trailing)
{
    int x_pos;
    StringValue(text);

    pango_glyph_string_index_to_x(_SELF(self),
                                  /* NOTE: This is a bug in the signature of
                                   * pango_glyph_string_index_to_x */
                                  (char *)RSTRING_PTR(text),
                                  RSTRING_LEN(text),
                                  (PangoAnalysis*)RVAL2BOXED(analysis, PANGO_TYPE_ANALYSIS),
                                  NUM2INT(index), RVAL2CBOOL(trailing),
                                  &x_pos);
    return INT2NUM(x_pos);
}

static VALUE
rglyph_x_to_index(VALUE self, VALUE text, VALUE analysis, VALUE x_pos)
{
    int index;
    int trailing;

    StringValue(text);
    pango_glyph_string_x_to_index(_SELF(self),
                                  /* NOTE: This is a bug in the signature of
                                   * pango_glyph_string_index_to_x */
                                  (char *)RSTRING_PTR(text),
                                  RSTRING_LEN(text),
                                  (PangoAnalysis*)RVAL2BOXED(analysis, PANGO_TYPE_ANALYSIS),
                                  NUM2INT(x_pos),
                                  &index, &trailing);
    return rb_assoc_new(INT2NUM(index), CBOOL2RVAL(trailing));
}

static VALUE
rglyph_get_logical_widths(VALUE self, VALUE rbtext, VALUE rbembedding_level)
{
    PangoGlyphString *glyphs = _SELF(self);
    const char *text = RVAL2CSTR(rbtext);
    long length = RSTRING_LEN(rbtext);
    int embedding_level = NUM2INT(rbembedding_level);
    glong n = g_utf8_strlen(text, length);
    int *logical_widths = g_new(int, n);

    pango_glyph_string_get_logical_widths(glyphs, text, length, embedding_level,
                                          logical_widths);

    return GINTS2RVAL_FREE(logical_widths, n);
}

static VALUE
rglyph_get_glyphs(VALUE self)
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
Init_pango_glyph_string(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_GLYPH_STRING, "GlyphString", mPango);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", rglyph_initialize, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_size", rglyph_set_size, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "extents", rglyph_extents, -1);
#if PANGO_CHECK_VERSION(1,14,0)
    rb_define_method(RG_TARGET_NAMESPACE, "width", rglyph_get_width, 0);
#endif
    rb_define_method(RG_TARGET_NAMESPACE, "index_to_x", rglyph_index_to_x, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "x_to_index", rglyph_x_to_index, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "get_logical_widths", rglyph_get_logical_widths, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "glyphs", rglyph_get_glyphs, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
