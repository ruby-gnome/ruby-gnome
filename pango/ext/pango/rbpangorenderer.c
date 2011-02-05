/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangorenderer.c -

  $Author: ggc $
  $Date: 2006/06/22 19:52:54 $

  Copyright (C) 2005 Masao Mutoh 
************************************************/

#include "rbpango.h"

#if PANGO_CHECK_VERSION(1,8,0)
#define _SELF(self) (PANGO_RENDERER(RVAL2GOBJ(self)))

static VALUE
renderer_draw_layout(VALUE self, VALUE layout, VALUE x, VALUE y)
{
    pango_renderer_draw_layout(_SELF(self), 
                               PANGO_LAYOUT(RVAL2GOBJ(layout)),
                               NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
renderer_draw_layout_line(VALUE self, VALUE line, VALUE x, VALUE y)
{
    pango_renderer_draw_layout_line(_SELF(self),  
                                    (PangoLayoutLine*)RVAL2BOXED(line, PANGO_TYPE_LAYOUT_LINE),
                                    NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
renderer_draw_glyphs(VALUE self, VALUE font, VALUE glyphs, VALUE x, VALUE y)
{
    pango_renderer_draw_glyphs(_SELF(self), 
                               PANGO_FONT(RVAL2GOBJ(self)),
                               (PangoGlyphString*)(RVAL2BOXED(self, PANGO_TYPE_GLYPH_STRING)),
                               NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
renderer_draw_rectangle(VALUE self, VALUE part, VALUE x, VALUE y, VALUE width, VALUE height)
{
#if HAVE_PANGO_RENDER_PART_GET_TYPE
    pango_renderer_draw_rectangle(_SELF(self), RVAL2GENUM(part, PANGO_TYPE_RENDER_PART),
                                  NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
#else
    pango_renderer_draw_rectangle(_SELF(self), NUM2INT(part),
                                  NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
#endif
    return self;
}

static VALUE
renderer_draw_error_underline(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    pango_renderer_draw_error_underline(_SELF(self),
                                        NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
renderer_draw_trapezoid(VALUE self, VALUE part, VALUE y1, VALUE x11, VALUE x21, VALUE y2, VALUE x12, VALUE x22)
{
#if HAVE_PANGO_RENDER_PART_GET_TYPE
    pango_renderer_draw_trapezoid(_SELF(self), 
                                  RVAL2GENUM(part, PANGO_TYPE_RENDER_PART),
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
renderer_draw_glyph(VALUE self, VALUE font, VALUE glyph, VALUE x, VALUE y)
{
    pango_renderer_draw_glyph(_SELF(self), PANGO_FONT(RVAL2GOBJ(self)),
                              NUM2INT(glyph), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
renderer_deactivate(VALUE self)
{
    pango_renderer_deactivate(_SELF(self));
    return self;
}

static VALUE
renderer_activate(VALUE self)
{
    pango_renderer_activate(_SELF(self));
    if (rb_block_given_p()) {
        rb_ensure(rb_yield, self, renderer_deactivate, self);
    }
    return self;
}

static VALUE
renderer_part_changed(VALUE self, VALUE part)
{
#if HAVE_PANGO_RENDER_PART_GET_TYPE
    pango_renderer_part_changed(_SELF(self), RVAL2GENUM(part, PANGO_TYPE_RENDER_PART));
#else
    pango_renderer_part_changed(_SELF(self), NUM2INT(part));
#endif
    return self;
}

static VALUE
renderer_set_color(VALUE self, VALUE part, VALUE color)
{
#if HAVE_PANGO_RENDER_PART_GET_TYPE
    pango_renderer_set_color(_SELF(self), RVAL2GENUM(part, PANGO_TYPE_RENDER_PART),
                             (PangoColor*)(NIL_P(color) ? NULL : RVAL2BOXED(color, PANGO_TYPE_COLOR)));
#else
    pango_renderer_set_color(_SELF(self), NUM2INT(part),
                             (PangoColor*)(NIL_P(color) ? NULL : RVAL2BOXED(color, PANGO_TYPE_COLOR)));
#endif
    return self;
}

static VALUE
renderer_get_color(VALUE self, VALUE part)
{
#if HAVE_PANGO_RENDER_PART_GET_TYPE
    PangoColor* color = pango_renderer_get_color(_SELF(self),
                                                 RVAL2GENUM(part, PANGO_TYPE_RENDER_PART));
#else
    PangoColor* color = pango_renderer_get_color(_SELF(self),NUM2INT(part));
#endif
    return BOXED2RVAL(color, PANGO_TYPE_COLOR);
}

static VALUE
renderer_set_matrix(VALUE self, VALUE matrix)
{
    pango_renderer_set_matrix(_SELF(self), 
                              (PangoMatrix*)(NIL_P(matrix) ? NULL : RVAL2BOXED(matrix, PANGO_TYPE_MATRIX)));
    return self;
}

static VALUE
renderer_get_matrix(VALUE self)
{
    const PangoMatrix* matrix = pango_renderer_get_matrix(_SELF(self));
    return BOXED2RVAL((PangoMatrix*)matrix, PANGO_TYPE_MATRIX);
}

#endif

void
Init_pangorenderer()
{
#if PANGO_CHECK_VERSION(1,8,0)
    VALUE renderer = G_DEF_CLASS(PANGO_TYPE_RENDERER, "Renderer", mPango);

    rb_define_method(renderer, "draw_layout", renderer_draw_layout, 3);
    rb_define_method(renderer, "draw_layout_line", renderer_draw_layout_line, 3);
    rb_define_method(renderer, "draw_glyphs", renderer_draw_glyphs, 4);
    rb_define_method(renderer, "draw_rectangle", renderer_draw_rectangle, 5);
    rb_define_method(renderer, "draw_error_underline", renderer_draw_error_underline, 4);
    rb_define_method(renderer, "draw_trapezoid", renderer_draw_trapezoid, 7);
    rb_define_method(renderer, "draw_glyph", renderer_draw_glyph, 4);
    rb_define_method(renderer, "activate", renderer_activate, 0);
    rb_define_method(renderer, "deactivate", renderer_deactivate, 0); 
    rb_define_method(renderer, "part_changed", renderer_part_changed, 1);
    rb_define_method(renderer, "set_color", renderer_set_color, 2);
    rb_define_method(renderer, "get_color", renderer_get_color, 1);
    rb_define_method(renderer, "set_matrix", renderer_set_matrix, 1);
    rb_define_method(renderer, "matrix", renderer_get_matrix, 0);

    G_DEF_SETTERS(renderer);

    /* PangoRenderPart */
#if HAVE_PANGO_RENDER_PART_GET_TYPE
    G_DEF_CLASS(PANGO_TYPE_RENDER_PART, "Part", renderer);
    G_DEF_CONSTANTS(renderer, PANGO_TYPE_RENDER_PART, "PANGO_RENDER_");
#endif

#endif
}
