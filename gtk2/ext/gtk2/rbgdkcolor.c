/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "global.h"

#define RG_TARGET_NAMESPACE rb_cGdkColor
#define _SELF(c) (RVAL2GDKCOLOR(c))

VALUE RG_TARGET_NAMESPACE = Qnil;

static VALUE
rg_initialize(VALUE self, VALUE red, VALUE green, VALUE blue)
{
    GdkColor c;
    c.pixel = 0;
    c.red = NUM2INT(red);
    c.green = NUM2INT(green);
    c.blue = NUM2INT(blue);

    G_INITIALIZE(self, &c);

    return Qnil;
}

static VALUE
rg_s_parse(G_GNUC_UNUSED VALUE self, VALUE name)
{
    GdkColor c;
    if (! gdk_color_parse(RVAL2CSTR(name), &c)) {
        rb_raise(rb_eArgError, "can't parse color name `%s'", RVAL2CSTR(name));
    }
    return BOXED2RVAL(&c, GDK_TYPE_COLOR);
}

static VALUE
rg_pixel(VALUE self)
{
    return INT2NUM(_SELF(self)->pixel);
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
    GdkColor *c = _SELF(self);
    return rb_ary_new3(3, INT2FIX(c->red), 
                       INT2FIX(c->green), INT2FIX(c->blue));
}

static VALUE
rg_operator_gdkcolor_equal(VALUE self, VALUE other)
{
    return CBOOL2RVAL(gdk_color_equal(_SELF(self), _SELF(other)));
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
rg_to_s(VALUE self)
{
    return CSTR2RVAL_FREE(gdk_color_to_string(_SELF(self)));
}
#endif

void
Init_gtk_gdk_color(VALUE mGdk)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_COLOR, "Color", mGdk);

    RG_DEF_SMETHOD(parse, 1);
    RG_DEF_METHOD(initialize, 3);
    RG_DEF_METHOD(pixel, 0);
    RG_DEF_METHOD(red, 0);
    RG_DEF_METHOD(set_red, 1);
    RG_DEF_METHOD(green, 0);
    RG_DEF_METHOD(set_green, 1);
    RG_DEF_METHOD(blue, 0);
    RG_DEF_METHOD(set_blue, 1);
    RG_DEF_METHOD(to_a, 0);
    RG_DEF_METHOD_OPERATOR("==", gdkcolor_equal, 1);
#if GTK_CHECK_VERSION(2, 12, 0)
    RG_DEF_METHOD(to_s, 0);
#endif

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
