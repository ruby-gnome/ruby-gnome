/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbgdk3private.h"

#define RG_TARGET_NAMESPACE cRGBA
#define _SELF(self) (RVAL2GDKRGBA(self))

static VALUE
rg_s_parse(G_GNUC_UNUSED VALUE self, VALUE spec)
{
    GdkRGBA color;
    if (!gdk_rgba_parse(&color, RVAL2CSTR(spec)))
        rb_raise(rb_eArgError, "can't parse color representation `%s'", RVAL2CSTR(spec));
    return GDKRGBA2RVAL(&color);
}

static VALUE
rg_initialize(VALUE self, VALUE red, VALUE green, VALUE blue, VALUE alpha)
{
    GdkRGBA rgba;

    rgba.red = NUM2DBL(red);
    rgba.green = NUM2DBL(green);
    rgba.blue = NUM2DBL(blue);
    rgba.alpha = NUM2DBL(alpha);

    G_INITIALIZE(self, g_boxed_copy(GDK_TYPE_RGBA, &rgba));

    return Qnil;
}

static VALUE
rg_red(VALUE self)
{
    return DBL2NUM(_SELF(self)->red);
}

static VALUE
rg_set_red(VALUE self, VALUE red)
{
    _SELF(self)->red = NUM2DBL(red);
    return self;
}

static VALUE
rg_green(VALUE self)
{
    return DBL2NUM(_SELF(self)->green);
}

static VALUE
rg_set_green(VALUE self, VALUE green)
{
    _SELF(self)->green = NUM2DBL(green);
    return self;
}

static VALUE
rg_blue(VALUE self)
{
    return DBL2NUM(_SELF(self)->blue);
}

static VALUE
rg_set_blue(VALUE self, VALUE blue)
{
    _SELF(self)->blue = NUM2DBL(blue);
    return self;
}

static VALUE
rg_alpha(VALUE self)
{
    return DBL2NUM(_SELF(self)->alpha);
}

static VALUE
rg_set_alpha(VALUE self, VALUE alpha)
{
    _SELF(self)->alpha = NUM2DBL(alpha);
    return self;
}

static VALUE
rg_to_a(VALUE self)
{
    GdkRGBA *rgba = _SELF(self);
    return rb_ary_new3(4, DBL2NUM(rgba->red), 
                          DBL2NUM(rgba->green),
                          DBL2NUM(rgba->blue),
                          DBL2NUM(rgba->alpha));
}

static VALUE
rg_to_s(VALUE self)
{
    return CSTR2RVAL_FREE(gdk_rgba_to_string(_SELF(self)));
}

static VALUE
rg_operator_equal(VALUE self, VALUE other)
{
    return CBOOL2RVAL(gdk_rgba_equal(_SELF(self), _SELF(other)));
}

void
Init_gdk_rgba(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_RGBA, "RGBA", mGdk);

    RG_DEF_SMETHOD(parse, 1);
    RG_DEF_METHOD(initialize, 4);
    RG_DEF_METHOD(red, 0);
    RG_DEF_METHOD(set_red, 1);
    RG_DEF_METHOD(green, 0);
    RG_DEF_METHOD(set_green, 1);
    RG_DEF_METHOD(blue, 0);
    RG_DEF_METHOD(set_blue, 1);
    RG_DEF_METHOD(alpha, 0);
    RG_DEF_METHOD(set_alpha, 1);
    RG_DEF_METHOD(to_a, 0);
    RG_DEF_METHOD(to_s, 0);
    RG_DEF_METHOD_OPERATOR("==", equal, 1);
}
