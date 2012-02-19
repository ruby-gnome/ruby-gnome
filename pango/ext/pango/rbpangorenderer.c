/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#if PANGO_CHECK_VERSION(1,8,0)
#define RG_TARGET_NAMESPACE cRenderer
#define _SELF(self) (RVAL2PANGORENDERER(self))

static VALUE
rg_draw_layout(VALUE self, VALUE layout, VALUE x, VALUE y)
{
    pango_renderer_draw_layout(_SELF(self), 
                               RVAL2PANGOLAYOUT(layout),
                               NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_draw_layout_line(VALUE self, VALUE line, VALUE x, VALUE y)
{
    pango_renderer_draw_layout_line(_SELF(self),  
                                    RVAL2PANGOLAYOUTLINE(line),
                                    NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_draw_glyphs(VALUE self, VALUE font, VALUE glyphs, VALUE x, VALUE y)
{
    pango_renderer_draw_glyphs(_SELF(self),
                               RVAL2PANGOFONT(font),
                               RVAL2PANGOGLYPHSTRING(glyphs),
                               NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_draw_rectangle(VALUE self, VALUE part, VALUE x, VALUE y, VALUE width, VALUE height)
{
#ifdef HAVE_PANGO_RENDER_PART_GET_TYPE
    pango_renderer_draw_rectangle(_SELF(self), RVAL2PANGORENDERPART(part),
                                  NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
#else
    pango_renderer_draw_rectangle(_SELF(self), NUM2INT(part),
                                  NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
#endif
    return self;
}

static VALUE
rg_draw_error_underline(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    pango_renderer_draw_error_underline(_SELF(self),
                                        NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_draw_trapezoid(VALUE self, VALUE part, VALUE y1, VALUE x11, VALUE x21, VALUE y2, VALUE x12, VALUE x22)
{
#ifdef HAVE_PANGO_RENDER_PART_GET_TYPE
    pango_renderer_draw_trapezoid(_SELF(self), 
                                  RVAL2PANGORENDERPART(part),
                                  NUM2DBL(y1), NUM2DBL(x11), NUM2DBL(x21),
                                  NUM2DBL(y2), NUM2DBL(x12), NUM2DBL(x22));
#else
    pango_renderer_draw_trapezoid(_SELF(self), NUM2INT(part),
                                  NUM2DBL(y1), NUM2DBL(x11), NUM2DBL(x21),
                                  NUM2DBL(y2), NUM2DBL(x12), NUM2DBL(x22));
#endif
    return self;
}

static VALUE
rg_draw_glyph(VALUE self, VALUE font, VALUE glyph, VALUE x, VALUE y)
{
    pango_renderer_draw_glyph(_SELF(self), RVAL2PANGOFONT(font),
                              NUM2INT(glyph), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_deactivate(VALUE self)
{
    pango_renderer_deactivate(_SELF(self));
    return self;
}

static VALUE
rg_activate(VALUE self)
{
    pango_renderer_activate(_SELF(self));
    if (rb_block_given_p()) {
        rb_ensure(rb_yield, self, rg_deactivate, self);
    }
    return self;
}

static VALUE
rg_part_changed(VALUE self, VALUE part)
{
#ifdef HAVE_PANGO_RENDER_PART_GET_TYPE
    pango_renderer_part_changed(_SELF(self), RVAL2PANGORENDERPART(part));
#else
    pango_renderer_part_changed(_SELF(self), NUM2INT(part));
#endif
    return self;
}

static VALUE
rg_set_color(VALUE self, VALUE part, VALUE color)
{
#ifdef HAVE_PANGO_RENDER_PART_GET_TYPE
    pango_renderer_set_color(_SELF(self), RVAL2PANGORENDERPART(part),
                             NIL_P(color) ? NULL : RVAL2PANGOCOLOR(color));
#else
    pango_renderer_set_color(_SELF(self), NUM2INT(part),
                             NIL_P(color) ? NULL : RVAL2PANGOCOLOR(color));
#endif
    return self;
}

static VALUE
rg_get_color(VALUE self, VALUE part)
{
#ifdef HAVE_PANGO_RENDER_PART_GET_TYPE
    PangoColor* color = pango_renderer_get_color(_SELF(self),
                                                 RVAL2PANGORENDERPART(part));
#else
    PangoColor* color = pango_renderer_get_color(_SELF(self),NUM2INT(part));
#endif
    return PANGOCOLOR2RVAL(color);
}

static VALUE
rg_set_matrix(VALUE self, VALUE matrix)
{
    pango_renderer_set_matrix(_SELF(self), 
                              NIL_P(matrix) ? NULL : RVAL2PANGOMATRIX(matrix));
    return self;
}

static VALUE
rg_matrix(VALUE self)
{
    const PangoMatrix* matrix = pango_renderer_get_matrix(_SELF(self));
    return PANGOMATRIX2RVAL((PangoMatrix*)matrix);
}

#endif

void
Init_pangorenderer(VALUE mPango)
{
#if PANGO_CHECK_VERSION(1,8,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_RENDERER, "Renderer", mPango);

    RG_DEF_METHOD(draw_layout, 3);
    RG_DEF_METHOD(draw_layout_line, 3);
    RG_DEF_METHOD(draw_glyphs, 4);
    RG_DEF_METHOD(draw_rectangle, 5);
    RG_DEF_METHOD(draw_error_underline, 4);
    RG_DEF_METHOD(draw_trapezoid, 7);
    RG_DEF_METHOD(draw_glyph, 4);
    RG_DEF_METHOD(activate, 0);
    RG_DEF_METHOD(deactivate, 0); 
    RG_DEF_METHOD(part_changed, 1);
    RG_DEF_METHOD(set_color, 2);
    RG_DEF_METHOD(get_color, 1);
    RG_DEF_METHOD(set_matrix, 1);
    RG_DEF_METHOD(matrix, 0);

    /* PangoRenderPart */
#ifdef HAVE_PANGO_RENDER_PART_GET_TYPE
    G_DEF_CLASS(PANGO_TYPE_RENDER_PART, "Part", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_RENDER_PART, "PANGO_RENDER_");
#endif

#endif
}
