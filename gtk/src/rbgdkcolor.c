/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkcolor.c -

  $Author: mutoh $
  $Date: 2002/07/06 20:56:14 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"


VALUE gdkColor;

static VALUE
gdkcolor_s_new(self, r, g, b)
    VALUE self;
    VALUE r, g, b;
{
    GdkColor c;
    c.pixel = 0;
    c.red = NUM2INT(r);
    c.green = NUM2INT(g);
    c.blue = NUM2INT(b);
    return make_gdkcolor(&c);
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
    return make_gdkcolor(&c);
}

static VALUE
gdkcolor_pixel(self)
    VALUE self;
{
    return INT2NUM(get_gdkcolor(self)->pixel);
}

static VALUE
gdkcolor_red(self)
    VALUE self;
{
    return INT2FIX(get_gdkcolor(self)->red);
}

static VALUE
gdkcolor_set_red(self, red)
    VALUE self;
    VALUE red;
{
    get_gdkcolor(self)->red = NUM2INT(red);
    return red;
}

static VALUE
gdkcolor_green(self)
    VALUE self;
{
    return INT2FIX(get_gdkcolor(self)->green);
}

static VALUE
gdkcolor_set_green(self, green)
    VALUE self;
    VALUE green;
{
    get_gdkcolor(self)->green = NUM2INT(green);
    return green;
}

static VALUE
gdkcolor_blue(self)
    VALUE self;
{
    return INT2FIX(get_gdkcolor(self)->blue);
}

static VALUE
gdkcolor_set_blue(self, blue)
    VALUE self;
    VALUE blue;
{
    get_gdkcolor(self)->blue = NUM2INT(blue);
    return blue;
}

static VALUE
gdkcolor_to_a(self)
    VALUE self;
{
    GdkColor *c = get_gdkcolor(self);
    return rb_ary_new3(3, INT2FIX(c->red), INT2FIX(c->green), INT2FIX(c->blue));
}

/*
 * Gdk::ColorContext
 */
VALUE gdkColorContext;

void
Init_gtk_gdk_color()
{
    /* Gdk::Color */
    gdkColor = rb_define_class_under(mGdk, "Color", rb_cData);

    rb_define_singleton_method(gdkColor, "new", gdkcolor_s_new, 3);
    rb_define_singleton_method(gdkColor, "parse", gdkcolor_s_parse, 1);
    rb_define_method(gdkColor, "pixel", gdkcolor_pixel, 0);
    rb_define_method(gdkColor, "red", gdkcolor_red, 0);
    rb_define_method(gdkColor, "red=", gdkcolor_set_red, 1);
    rb_define_method(gdkColor, "green", gdkcolor_green, 0);
    rb_define_method(gdkColor, "green=", gdkcolor_set_green, 1);
    rb_define_method(gdkColor, "blue", gdkcolor_blue, 0);
    rb_define_method(gdkColor, "blue=", gdkcolor_set_blue, 1);
    rb_define_method(gdkColor, "to_a", gdkcolor_to_a, 0);

    /* Gdk::ColorContext */
    gdkColorContext = rb_define_class_under(mGdk, "ColorContext", rb_cData);
}

