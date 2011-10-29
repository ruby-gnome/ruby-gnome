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
gdkcolor_initialize(VALUE self, VALUE red, VALUE green, VALUE blue)
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
gdkcolor_s_parse(G_GNUC_UNUSED VALUE self, VALUE name)
{
    GdkColor c;
    if (! gdk_color_parse(RVAL2CSTR(name), &c)) {
        rb_raise(rb_eArgError, "can't parse color name `%s'", RVAL2CSTR(name));
    }
    return BOXED2RVAL(&c, GDK_TYPE_COLOR);
}

static VALUE
gdkcolor_pixel(VALUE self)
{
    return INT2NUM(_SELF(self)->pixel);
}

static VALUE
gdkcolor_red(VALUE self)
{

    return INT2FIX(_SELF(self)->red);
}

static VALUE
gdkcolor_set_red(VALUE self, VALUE red)
{
    _SELF(self)->red = NUM2INT(red);
    return self;
}

static VALUE
gdkcolor_green(VALUE self)
{
    return INT2FIX(_SELF(self)->green);
}

static VALUE
gdkcolor_set_green(VALUE self, VALUE green)
{
    _SELF(self)->green = NUM2INT(green);
    return self;
}

static VALUE
gdkcolor_blue(VALUE self)
{
    return INT2FIX(_SELF(self)->blue);
}

static VALUE
gdkcolor_set_blue(VALUE self, VALUE blue)
{
    _SELF(self)->blue = NUM2INT(blue);
    return self;
}

static VALUE
gdkcolor_to_a(VALUE self)
{
    GdkColor *c = _SELF(self);
    return rb_ary_new3(3, INT2FIX(c->red), 
                       INT2FIX(c->green), INT2FIX(c->blue));
}

static VALUE
gdkcolor_equal(VALUE self, VALUE other)
{
    return CBOOL2RVAL(gdk_color_equal(_SELF(self), _SELF(other)));
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
gdkcolor_to_string(VALUE self)
{
    return CSTR2RVAL_FREE(gdk_color_to_string(_SELF(self)));
}
#endif

void
Init_gtk_gdk_color(void)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_COLOR, "Color", mGdk);

    rb_define_singleton_method(RG_TARGET_NAMESPACE, "parse", gdkcolor_s_parse, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "initialize", gdkcolor_initialize, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "pixel", gdkcolor_pixel, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "red", gdkcolor_red, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_red", gdkcolor_set_red, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "green", gdkcolor_green, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_green", gdkcolor_set_green, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "blue", gdkcolor_blue, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_blue", gdkcolor_set_blue, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "to_a", gdkcolor_to_a, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "==", gdkcolor_equal, 1);
#if GTK_CHECK_VERSION(2, 12, 0)
    rb_define_method(RG_TARGET_NAMESPACE, "to_s", gdkcolor_to_string, 0);
#endif

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}

