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

#ifdef CAIRO_AVAILABLE

#define RG_TARGET_NAMESPACE cCairoFontMap
#define _SELF(self) (RVAL2PANGOCAIROFONTMAP(self))

static VALUE
rg_s_create(int argc, VALUE *argv, G_GNUC_UNUSED VALUE klass)
{
    VALUE rb_font_type;
    PangoFontMap *font_map = NULL;

    rb_scan_args(argc, argv, "01", &rb_font_type);
    if (NIL_P(rb_font_type)) {
        font_map = pango_cairo_font_map_new();
    } else {
        cairo_font_type_t font_type = CAIRO_FONT_TYPE_USER;
        if (rbgutil_key_equal(rb_font_type, "ft") ||
            rbgutil_key_equal(rb_font_type, "freetype")) {
            font_type = CAIRO_FONT_TYPE_FT;
        } else if (rbgutil_key_equal(rb_font_type, "win32")) {
            font_type = CAIRO_FONT_TYPE_WIN32;
        } else if (rbgutil_key_equal(rb_font_type, "quartz")) {
            font_type = CAIRO_FONT_TYPE_QUARTZ;
        } else {
            rb_raise(rb_eArgError,
                     "font type must be one of "
                     ":ft, :freetype, :win32 or :quartz: %s",
                     RBG_INSPECT(rb_font_type));
        }
        font_map = pango_cairo_font_map_new_for_font_type(font_type);
    }

    return GOBJ2RVAL(font_map);
}

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE klass)
{
    return GOBJ2RVAL(pango_cairo_font_map_get_default());
}

#  if PANGO_CHECK_VERSION(1, 22, 0)
static VALUE
rg_s_set_default(VALUE klass, VALUE font_map)
{
    pango_cairo_font_map_set_default(RVAL2GOBJ(font_map));
    return klass;
}
#  endif

static VALUE
rg_set_resolution(VALUE self, VALUE dpi)
{
    pango_cairo_font_map_set_resolution(_SELF(self), NUM2DBL(dpi));
    return self;
}

static VALUE
rg_resolution(VALUE self)
{
    return rb_float_new(pango_cairo_font_map_get_resolution(_SELF(self)));
}

static VALUE
rg_create_context(VALUE self)
{
    return GOBJ2RVAL_UNREF(pango_cairo_font_map_create_context(_SELF(self)));
}

#endif

void
Init_pango_cairo(VALUE mPango)
{
#ifdef CAIRO_AVAILABLE
    VALUE RG_TARGET_NAMESPACE;

    /* Pango::CairoFontMap */
    RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_CAIRO_FONT_MAP, "CairoFontMap", mPango);

    RG_DEF_SMETHOD(create, -1);
    RG_DEF_SMETHOD(default, 0);
#  if PANGO_CHECK_VERSION(1, 22, 0)
    RG_DEF_SMETHOD(set_default, 1);
#  endif

    RG_DEF_METHOD(set_resolution, 1);
    RG_DEF_METHOD(resolution, 0);
    RG_DEF_METHOD(create_context, 0);
#endif
}
