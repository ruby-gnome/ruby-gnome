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
rg_s_create(G_GNUC_UNUSED VALUE klass)
{
    return GOBJ2RVAL(pango_cairo_font_map_new());
}

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE klass)
{
    return GOBJ2RVAL(pango_cairo_font_map_get_default());
}

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

    RG_DEF_SMETHOD(create, 0);
    RG_DEF_SMETHOD(default, 0);

    RG_DEF_METHOD(set_resolution, 1);
    RG_DEF_METHOD(resolution, 0);
    RG_DEF_METHOD(create_context, 0);
#endif
}
