/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkcolor.c -

  $Author: mutoh $
  $Date: 2002/08/29 04:44:30 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(c) ((GdkColor*)RVAL2BOXED(c))

static VALUE
gdkcolor_initialize(self, red, green, blue)
    VALUE self;
    VALUE red, green, blue;
{
	GdkColor c;
    c.pixel = 0;
    c.red = NUM2INT(red);
    c.green = NUM2INT(green);
    c.blue = NUM2INT(blue);

	RBGOBJ_INITIALIZE(self, &c);

    return Qnil;
}

static VALUE
gdkcolor_s_parse(self, name)
    VALUE self;
    VALUE name;
{
    GdkColor c;
    if (! gdk_color_parse(STR2CSTR(name), &c)) {
        rb_raise(rb_eArgError, "can't parse color name `%s'", STR2CSTR(name));
    }
    return BOXED2RVAL(&c, GDK_TYPE_COLOR);
}

static VALUE
gdkcolor_pixel(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->pixel);
}

static VALUE
gdkcolor_red(self)
    VALUE self;
{

    return INT2FIX(_SELF(self)->red);
}

static VALUE
gdkcolor_set_red(self, red)
    VALUE self;
    VALUE red;
{
    _SELF(self)->red = NUM2INT(red);
    return red;
}

static VALUE
gdkcolor_green(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->green);
}

static VALUE
gdkcolor_set_green(self, green)
    VALUE self;
    VALUE green;
{
    _SELF(self)->green = NUM2INT(green);
    return green;
}

static VALUE
gdkcolor_blue(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->blue);
}

static VALUE
gdkcolor_set_blue(self, blue)
    VALUE self;
    VALUE blue;
{
    _SELF(self)->blue = NUM2INT(blue);
    return blue;
}

static VALUE
gdkcolor_to_a(self)
    VALUE self;
{
    GdkColor *c = _SELF(self);
    return rb_ary_new3(3, INT2FIX(c->red), 
					   INT2FIX(c->green), INT2FIX(c->blue));
}

static VALUE
gdkcolor_equal(self, other)
	VALUE self, other;
{
	return (gdk_color_equal(_SELF(self), _SELF(other)))? Qtrue : Qfalse;
}

void
Init_gtk_gdk_color()
{
	VALUE gdkColor = G_DEF_CLASS(GDK_TYPE_COLOR, "Color", mGdk);

    rb_define_singleton_method(gdkColor, "parse", gdkcolor_s_parse, 1);
    rb_define_method(gdkColor, "initialize", gdkcolor_initialize, 3);
    rb_define_method(gdkColor, "pixel", gdkcolor_pixel, 0);
    rb_define_method(gdkColor, "red", gdkcolor_red, 0);
    rb_define_method(gdkColor, "red=", gdkcolor_set_red, 1);
    rb_define_method(gdkColor, "green", gdkcolor_green, 0);
    rb_define_method(gdkColor, "green=", gdkcolor_set_green, 1);
    rb_define_method(gdkColor, "blue", gdkcolor_blue, 0);
    rb_define_method(gdkColor, "blue=", gdkcolor_set_blue, 1);
    rb_define_method(gdkColor, "to_a", gdkcolor_to_a, 0);
    rb_define_method(gdkColor, "==", gdkcolor_equal, 1);
}

