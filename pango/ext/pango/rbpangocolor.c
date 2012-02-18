/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cColor
#define _SELF(self) (RVAL2PANGOCOLOR(self))

static VALUE
rg_initialize(VALUE self, VALUE red, VALUE green, VALUE blue)
{
    PangoColor c;
    c.red = NUM2INT(red);
    c.green = NUM2INT(green);
    c.blue = NUM2INT(blue);

    G_INITIALIZE(self, &c);
    return Qnil;
}

static VALUE
rg_parse(VALUE self, VALUE spec)
{
    return CBOOL2RVAL(pango_color_parse(_SELF(self), RVAL2CSTR(spec)));
}

static VALUE
rg_red(VALUE self)
{
    return INT2FIX(_SELF(self)->red);
}

static VALUE
rg_set_red(VALUE self, VALUE red)
{
    _SELF(self)->red = NUM2INT(red);
    return self;
}

static VALUE
rg_green(VALUE self)
{
    return INT2FIX(_SELF(self)->green);
}

static VALUE
rg_set_green(VALUE self, VALUE green)
{
    _SELF(self)->green = NUM2INT(green);
    return self;
}

static VALUE
rg_blue(VALUE self)
{
    return INT2FIX(_SELF(self)->blue);
}

static VALUE
rg_set_blue(VALUE self, VALUE blue)
{
    _SELF(self)->blue = NUM2INT(blue);
    return self;
}

static VALUE
rg_to_a(VALUE self)
{
    PangoColor *c = _SELF(self);
    return rb_ary_new3(3, INT2FIX(c->red), 
                       INT2FIX(c->green), INT2FIX(c->blue));
}

static VALUE
rg_operator_color_equal(VALUE self, VALUE other)
{
    PangoColor* c1 = _SELF(self);
    PangoColor* c2 = _SELF(other);

    return CBOOL2RVAL((c1->red == c2->red) && (c1->green == c2->green) && 
                      (c1->blue == c2->blue));
}

void
Init_pango_color(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_COLOR, "Color", mPango);

    RG_DEF_METHOD(initialize, 3);
    RG_DEF_METHOD(parse, 1);
    RG_DEF_METHOD(red, 0);
    RG_DEF_METHOD(set_red, 1);
    RG_DEF_METHOD(green, 0);
    RG_DEF_METHOD(set_green, 1);
    RG_DEF_METHOD(blue, 0);
    RG_DEF_METHOD(set_blue, 1);
    RG_DEF_METHOD(to_a, 0);
    RG_DEF_METHOD_OPERATOR("==", color_equal, 1);
}
