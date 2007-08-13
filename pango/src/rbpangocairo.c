/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangocairo.c -

  $Author: ktou $
  $Date: 2007/08/13 11:10:22 $

  Copyright (C) 2005 Kouhei Sutou
  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "rbpango.h"

#if PANGO_CHECK_VERSION(1,10,0) && defined(HAVE_RB_CAIRO_H)
#  define CAIRO_AVAILABLE 1
#endif

#ifdef CAIRO_AVAILABLE

#define _SELF(self) (PANGO_CAIRO_FONT_MAP(RVAL2GOBJ(self)))
#define RVAL2CONTEXT(v) (PANGO_CONTEXT(RVAL2GOBJ(v)))
#define RVAL2LAYOUT(v) (PANGO_LAYOUT(RVAL2GOBJ(v)))
#define RVAL2FONT(v) (PANGO_FONT(RVAL2GOBJ(v)))
#define RVAL2GLYPH(v) ((PangoGlyphString*)(RVAL2BOXED(self, PANGO_TYPE_GLYPH_STRING)))
#define RVAL2LINE(v) ((PangoLayoutLine*)RVAL2BOXED(v, PANGO_TYPE_LAYOUT_LINE))

static VALUE
font_map_create(klass)
    VALUE klass;
{
    return GOBJ2RVAL(pango_cairo_font_map_new());
}

static VALUE
font_map_get_default(klass)
    VALUE klass;
{
    return GOBJ2RVAL(pango_cairo_font_map_get_default());
}

static VALUE
font_map_set_resolution(self, dpi)
    VALUE self, dpi;
{
    pango_cairo_font_map_set_resolution(_SELF(self), NUM2DBL(dpi));
    return self;
}

static VALUE
font_map_get_resolution(self)
    VALUE self;
{
    return rb_float_new(pango_cairo_font_map_get_resolution(_SELF(self)));
}

static VALUE
font_map_create_context(self)
    VALUE self;
{
    return GOBJ2RVAL(pango_cairo_font_map_create_context(_SELF(self)));
}

static VALUE
update_context(self, context)
    VALUE self, context;
{
    pango_cairo_update_context(RVAL2CRCONTEXT(self), RVAL2CONTEXT(context));
    return self;
}

/* Convenience */
static VALUE
create_layout(self)
    VALUE self;
{
    return GOBJ2RVAL(pango_cairo_create_layout(RVAL2CRCONTEXT(self)));
}

static VALUE
update_layout(self, layout)
    VALUE self, layout;
{
    pango_cairo_update_layout(RVAL2CRCONTEXT(self), RVAL2LAYOUT(layout));
    return self;
}

/* Rendering */
static VALUE
show_glyph_string(self, font, glyphs)
    VALUE self, font, glyphs;
{
    pango_cairo_show_glyph_string(RVAL2CRCONTEXT(self),
                                  RVAL2FONT(font),
                                  RVAL2GLYPH(glyphs));
    return self;
}

static VALUE
show_layout_line(self, line)
    VALUE self, line;
{
    pango_cairo_show_layout_line(RVAL2CRCONTEXT(self), RVAL2LINE(line));
    return self;
}

static VALUE
show_layout(self, layout)
    VALUE self, layout;
{
    pango_cairo_show_layout(RVAL2CRCONTEXT(self), RVAL2LAYOUT(layout));
    return self;
}

#if PANGO_CHECK_VERSION(1,14,0)
static VALUE
show_error_underline(self, x, y, width, height)
    VALUE self, x, y, width, height;
{
    pango_cairo_show_error_underline(RVAL2CRCONTEXT(self), 
                                     NUM2DBL(x), NUM2DBL(y), 
                                     NUM2DBL(width), NUM2DBL(height));
    return self;
}
#endif

/* Rendering to a path */
static VALUE
glyph_string_path(self, font, glyphs)
    VALUE self, font, glyphs;
{
    pango_cairo_glyph_string_path(RVAL2CRCONTEXT(self),
                                  RVAL2FONT(font),
                                  RVAL2GLYPH(glyphs));
    return self;
}

static VALUE
layout_line_path(self, line)
    VALUE self, line;
{
    pango_cairo_layout_line_path(RVAL2CRCONTEXT(self), RVAL2LINE(line));
    return self;
}

static VALUE
layout_path(self, layout)
    VALUE self, layout;
{
    pango_cairo_layout_path(RVAL2CRCONTEXT(self), RVAL2LAYOUT(layout));
    return self;
}

#if PANGO_CHECK_VERSION(1,14,0)
static VALUE
error_underline_path(self, x, y, width, height)
    VALUE self, x, y, width, height;
{
    pango_cairo_error_underline_path(RVAL2CRCONTEXT(self), 
                                     NUM2DBL(x), NUM2DBL(y), 
                                     NUM2DBL(width), NUM2DBL(height));
    return self;
}
#endif

#endif

static VALUE
cairo_available_p(VALUE self)
{
#if CAIRO_AVAILABLE
    return Qtrue;
#else
    return Qfalse;
#endif
}

void
Init_pango_cairo()
{
#ifdef CAIRO_AVAILABLE
    VALUE pFontMap;

    /* Pango::CairoFontMap */
    pFontMap = G_DEF_CLASS(PANGO_TYPE_CAIRO_FONT_MAP, "CairoFontMap", mPango);

    rb_define_singleton_method(pFontMap, "create", font_map_create, 0);
    rb_define_singleton_method(pFontMap, "default", font_map_get_default, 0);

    rb_define_method(pFontMap, "set_resolution", font_map_set_resolution, 1);
    rb_define_method(pFontMap, "resolution", font_map_get_resolution, 0);
    rb_define_method(pFontMap, "create_context", font_map_create_context, 0);

    G_DEF_SETTERS(pFontMap);

    /* Cairo::Context */
    rb_define_method(rb_cCairo_Context, "update_pango_context",
                     update_context, 1);
    /* Convenience */
    rb_define_method(rb_cCairo_Context, "create_pango_layout",
                     create_layout, 0);
    rb_define_method(rb_cCairo_Context, "update_pango_layout",
                     update_layout, 1);
    /* Rendering */
    rb_define_method(rb_cCairo_Context, "show_pango_glyph_string",
                     show_glyph_string, 2);
    rb_define_method(rb_cCairo_Context, "show_pango_layout_line",
                     show_layout_line, 1);
    rb_define_method(rb_cCairo_Context, "show_pango_layout",
                     show_layout, 1);
#if PANGO_CHECK_VERSION(1,14,0)
    rb_define_method(rb_cCairo_Context, "show_pango_error_underline",
                     show_error_underline, 4);
#endif
    /* Rendering to a path */
    rb_define_method(rb_cCairo_Context, "pango_glyph_string_path",
                     glyph_string_path, 2);
    rb_define_method(rb_cCairo_Context, "pango_layout_line_path",
                     layout_line_path, 1);
    rb_define_method(rb_cCairo_Context, "pango_layout_path",
                     layout_path, 1);

#if PANGO_CHECK_VERSION(1,14,0)
    rb_define_method(rb_cCairo_Context, "pango_error_underline_path",
                     error_underline_path, 4);
#endif

#endif

    rb_define_module_function(mPango, "cairo_available?", cairo_available_p, 0);
}
