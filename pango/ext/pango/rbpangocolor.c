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
#define _SELF(self) ((PangoColor*)RVAL2BOXED(self, PANGO_TYPE_COLOR))

static VALUE
color_initialize(VALUE self, VALUE red, VALUE green, VALUE blue)
{
    PangoColor c;
    c.red = NUM2INT(red);
    c.green = NUM2INT(green);
    c.blue = NUM2INT(blue);

    G_INITIALIZE(self, &c);
    return Qnil;
}

static VALUE
color_parse(VALUE self, VALUE spec)
{
    return CBOOL2RVAL(pango_color_parse(_SELF(self), RVAL2CSTR(spec)));
}

static VALUE
color_red(VALUE self)
{
    return INT2FIX(_SELF(self)->red);
}

static VALUE
color_set_red(VALUE self, VALUE red)
{
    _SELF(self)->red = NUM2INT(red);
    return self;
}

static VALUE
color_green(VALUE self)
{
    return INT2FIX(_SELF(self)->green);
}

static VALUE
color_set_green(VALUE self, VALUE green)
{
    _SELF(self)->green = NUM2INT(green);
    return self;
}

static VALUE
color_blue(VALUE self)
{
    return INT2FIX(_SELF(self)->blue);
}

static VALUE
color_set_blue(VALUE self, VALUE blue)
{
    _SELF(self)->blue = NUM2INT(blue);
    return self;
}

static VALUE
color_to_a(VALUE self)
{
    PangoColor *c = _SELF(self);
    return rb_ary_new3(3, INT2FIX(c->red), 
                       INT2FIX(c->green), INT2FIX(c->blue));
}

static VALUE
color_equal(VALUE self, VALUE other)
{
    PangoColor* c1 = _SELF(self);
    PangoColor* c2 = _SELF(other);

    return CBOOL2RVAL((c1->red == c2->red) && (c1->green == c2->green) && 
                      (c1->blue == c2->blue));
}

void
Init_pango_color(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_COLOR, "Color", mPango);
    
    rb_define_method(RG_TARGET_NAMESPACE, "initialize", color_initialize, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "parse", color_parse, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "red", color_red, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_red", color_set_red, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "green", color_green, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_green", color_set_green, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "blue", color_blue, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_blue", color_set_blue, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "to_a", color_to_a, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "==", color_equal, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
