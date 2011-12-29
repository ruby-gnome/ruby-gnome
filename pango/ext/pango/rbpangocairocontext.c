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

static VALUE
rg_update_pango_context(VALUE self, VALUE context)
{
    pango_cairo_update_context(RVAL2CRCONTEXT(self), RVAL2PANGOCONTEXT(context));
    return self;
}

/* Convenience */
static VALUE
rg_create_pango_layout(VALUE self)
{
    return GOBJ2RVAL_UNREF(pango_cairo_create_layout(RVAL2CRCONTEXT(self)));
}

static VALUE
rg_update_pango_layout(VALUE self, VALUE layout)
{
    pango_cairo_update_layout(RVAL2CRCONTEXT(self), RVAL2PANGOLAYOUT(layout));
    return self;
}

/* Rendering */
static VALUE
rg_show_pango_glyph_string(VALUE self, VALUE font, VALUE glyphs)
{
    pango_cairo_show_glyph_string(RVAL2CRCONTEXT(self),
                                  RVAL2PANGOFONT(font),
                                  RVAL2PANGOGLYPHSTRING(glyphs));
    return self;
}

static VALUE
rg_show_pango_layout_line(VALUE self, VALUE line)
{
    pango_cairo_show_layout_line(RVAL2CRCONTEXT(self), RVAL2PANGOLAYOUTLINE(line));
    return self;
}

static VALUE
rg_show_pango_layout(VALUE self, VALUE layout)
{
    pango_cairo_show_layout(RVAL2CRCONTEXT(self), RVAL2PANGOLAYOUT(layout));
    return self;
}

#if PANGO_CHECK_VERSION(1,14,0)
static VALUE
rg_show_pango_error_underline(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    pango_cairo_show_error_underline(RVAL2CRCONTEXT(self), 
                                     NUM2DBL(x), NUM2DBL(y), 
                                     NUM2DBL(width), NUM2DBL(height));
    return self;
}
#endif

/* Rendering to a path */
static VALUE
rg_pango_glyph_string_path(VALUE self, VALUE font, VALUE glyphs)
{
    pango_cairo_glyph_string_path(RVAL2CRCONTEXT(self),
                                  RVAL2PANGOFONT(font),
                                  RVAL2PANGOGLYPHSTRING(glyphs));
    return self;
}

static VALUE
rg_pango_layout_line_path(VALUE self, VALUE line)
{
    pango_cairo_layout_line_path(RVAL2CRCONTEXT(self), RVAL2PANGOLAYOUTLINE(line));
    return self;
}

static VALUE
rg_pango_layout_path(VALUE self, VALUE layout)
{
    pango_cairo_layout_path(RVAL2CRCONTEXT(self), RVAL2PANGOLAYOUT(layout));
    return self;
}

#if PANGO_CHECK_VERSION(1,14,0)
static VALUE
rg_pango_error_underline_path(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
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
    RG_DEF_METHOD(update_pango_context, 1);
    /* Convenience */
    RG_DEF_METHOD(create_pango_layout, 0);
    RG_DEF_METHOD(update_pango_layout, 1);
    /* Rendering */
    RG_DEF_METHOD(show_pango_glyph_string, 2);
    RG_DEF_METHOD(show_pango_layout_line, 1);
    RG_DEF_METHOD(show_pango_layout, 1);
#if PANGO_CHECK_VERSION(1,14,0)
    RG_DEF_METHOD(show_pango_error_underline, 4);
#endif
    /* Rendering to a path */
    RG_DEF_METHOD(pango_glyph_string_path, 2);
    RG_DEF_METHOD(pango_layout_line_path, 1);
    RG_DEF_METHOD(pango_layout_path, 1);

#if PANGO_CHECK_VERSION(1,14,0)
    RG_DEF_METHOD(pango_error_underline_path, 4);
#endif
#endif
}
