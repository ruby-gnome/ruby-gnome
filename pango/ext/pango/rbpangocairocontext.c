/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Kouhei Sutou
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

#if PANGO_CHECK_VERSION(1,10,0) && defined(HAVE_RB_CAIRO_H)
#  define CAIRO_AVAILABLE 1
#endif

#ifdef CAIRO_AVAILABLE

#define RG_TARGET_NAMESPACE rb_cCairo_Context

#define RVAL2CONTEXT(v) (PANGO_CONTEXT(RVAL2GOBJ(v)))
#define RVAL2LAYOUT(v) (PANGO_LAYOUT(RVAL2GOBJ(v)))
#define RVAL2FONT(v) (PANGO_FONT(RVAL2GOBJ(v)))
#define RVAL2GLYPH(v) ((PangoGlyphString*)(RVAL2BOXED(self, PANGO_TYPE_GLYPH_STRING)))
#define RVAL2LINE(v) ((PangoLayoutLine*)RVAL2BOXED(v, PANGO_TYPE_LAYOUT_LINE))

static VALUE
update_context(VALUE self, VALUE context)
{
    pango_cairo_update_context(RVAL2CRCONTEXT(self), RVAL2CONTEXT(context));
    return self;
}

/* Convenience */
static VALUE
create_layout(VALUE self)
{
    return GOBJ2RVAL_UNREF(pango_cairo_create_layout(RVAL2CRCONTEXT(self)));
}

static VALUE
update_layout(VALUE self, VALUE layout)
{
    pango_cairo_update_layout(RVAL2CRCONTEXT(self), RVAL2LAYOUT(layout));
    return self;
}

/* Rendering */
static VALUE
show_glyph_string(VALUE self, VALUE font, VALUE glyphs)
{
    pango_cairo_show_glyph_string(RVAL2CRCONTEXT(self),
                                  RVAL2FONT(font),
                                  RVAL2GLYPH(glyphs));
    return self;
}

static VALUE
show_layout_line(VALUE self, VALUE line)
{
    pango_cairo_show_layout_line(RVAL2CRCONTEXT(self), RVAL2LINE(line));
    return self;
}

static VALUE
show_layout(VALUE self, VALUE layout)
{
    pango_cairo_show_layout(RVAL2CRCONTEXT(self), RVAL2LAYOUT(layout));
    return self;
}

#if PANGO_CHECK_VERSION(1,14,0)
static VALUE
show_error_underline(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    pango_cairo_show_error_underline(RVAL2CRCONTEXT(self), 
                                     NUM2DBL(x), NUM2DBL(y), 
                                     NUM2DBL(width), NUM2DBL(height));
    return self;
}
#endif

/* Rendering to a path */
static VALUE
glyph_string_path(VALUE self, VALUE font, VALUE glyphs)
{
    pango_cairo_glyph_string_path(RVAL2CRCONTEXT(self),
                                  RVAL2FONT(font),
                                  RVAL2GLYPH(glyphs));
    return self;
}

static VALUE
layout_line_path(VALUE self, VALUE line)
{
    pango_cairo_layout_line_path(RVAL2CRCONTEXT(self), RVAL2LINE(line));
    return self;
}

static VALUE
layout_path(VALUE self, VALUE layout)
{
    pango_cairo_layout_path(RVAL2CRCONTEXT(self), RVAL2LAYOUT(layout));
    return self;
}

#if PANGO_CHECK_VERSION(1,14,0)
static VALUE
error_underline_path(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    pango_cairo_error_underline_path(RVAL2CRCONTEXT(self), 
                                     NUM2DBL(x), NUM2DBL(y), 
                                     NUM2DBL(width), NUM2DBL(height));
    return self;
}
#endif

#endif

void
Init_pango_cairo_context(VALUE mPango)
{
#ifdef CAIRO_AVAILABLE
    /* Cairo::Context */
    rb_define_method(RG_TARGET_NAMESPACE, "update_pango_context",
                     update_context, 1);
    /* Convenience */
    rb_define_method(RG_TARGET_NAMESPACE, "create_pango_layout",
                     create_layout, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "update_pango_layout",
                     update_layout, 1);
    /* Rendering */
    rb_define_method(RG_TARGET_NAMESPACE, "show_pango_glyph_string",
                     show_glyph_string, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "show_pango_layout_line",
                     show_layout_line, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "show_pango_layout",
                     show_layout, 1);
#if PANGO_CHECK_VERSION(1,14,0)
    rb_define_method(RG_TARGET_NAMESPACE, "show_pango_error_underline",
                     show_error_underline, 4);
#endif
    /* Rendering to a path */
    rb_define_method(RG_TARGET_NAMESPACE, "pango_glyph_string_path",
                     glyph_string_path, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "pango_layout_line_path",
                     layout_line_path, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "pango_layout_path",
                     layout_path, 1);

#if PANGO_CHECK_VERSION(1,14,0)
    rb_define_method(RG_TARGET_NAMESPACE, "pango_error_underline_path",
                     error_underline_path, 4);
#endif
#endif
}

