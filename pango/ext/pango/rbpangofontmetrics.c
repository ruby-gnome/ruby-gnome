/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cFontMetrics
#define _SELF(self) (RVAL2PANGOFONTMETRICS(self))

static VALUE
rg_ascent(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_ascent(_SELF(self)));
}

static VALUE
rg_descent(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_descent(_SELF(self)));
}

static VALUE
rg_approximate_char_width(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_approximate_char_width(_SELF(self)));
}

static VALUE
rg_approximate_digit_width(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_approximate_digit_width(_SELF(self)));
}

#if PANGO_CHECK_VERSION(1,6,0)
static VALUE
rg_underline_thickness(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_underline_thickness(_SELF(self)));
}
static VALUE
rg_underline_position(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_underline_position(_SELF(self)));
}
static VALUE
rg_strikethrough_thickness(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_strikethrough_thickness(_SELF(self)));
}
static VALUE
rg_strikethrough_position(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_strikethrough_position(_SELF(self)));
}
#endif
void
Init_pango_font_metrics(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_FONT_METRICS, "FontMetrics", mPango);

    RG_DEF_METHOD(ascent, 0);
    RG_DEF_METHOD(descent, 0);
    RG_DEF_METHOD(approximate_char_width, 0);
    RG_DEF_METHOD(approximate_digit_width, 0);
#if PANGO_CHECK_VERSION(1,6,0)
    RG_DEF_METHOD(underline_thickness, 0);
    RG_DEF_METHOD(underline_position, 0);
    RG_DEF_METHOD(strikethrough_thickness, 0);
    RG_DEF_METHOD(strikethrough_position, 0);
#endif
}
