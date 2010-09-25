/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkcolor.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:08 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(c) (RVAL2GDKCOLOR(c))

VALUE rb_cGdkColor = Qnil;

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

    G_INITIALIZE(self, &c);

    return Qnil;
}

static VALUE
gdkcolor_s_parse(self, name)
    VALUE self;
    VALUE name;
{
    GdkColor c;
    if (! gdk_color_parse(RVAL2CSTR(name), &c)) {
        rb_raise(rb_eArgError, "can't parse color name `%s'", RVAL2CSTR(name));
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
    return self;
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
    return self;
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
    return self;
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
    return CBOOL2RVAL(gdk_color_equal(_SELF(self), _SELF(other)));
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
gdkcolor_to_string(VALUE self, VALUE other)
{
    return CSTR2RVAL_FREE(gdk_color_to_string(_SELF(self)));
}
#endif

void
Init_gtk_gdk_color()
{
    rb_cGdkColor = G_DEF_CLASS(GDK_TYPE_COLOR, "Color", mGdk);

    rb_define_singleton_method(rb_cGdkColor, "parse", gdkcolor_s_parse, 1);
    rb_define_method(rb_cGdkColor, "initialize", gdkcolor_initialize, 3);
    rb_define_method(rb_cGdkColor, "pixel", gdkcolor_pixel, 0);
    rb_define_method(rb_cGdkColor, "red", gdkcolor_red, 0);
    rb_define_method(rb_cGdkColor, "set_red", gdkcolor_set_red, 1);
    rb_define_method(rb_cGdkColor, "green", gdkcolor_green, 0);
    rb_define_method(rb_cGdkColor, "set_green", gdkcolor_set_green, 1);
    rb_define_method(rb_cGdkColor, "blue", gdkcolor_blue, 0);
    rb_define_method(rb_cGdkColor, "set_blue", gdkcolor_set_blue, 1);
    rb_define_method(rb_cGdkColor, "to_a", gdkcolor_to_a, 0);
    rb_define_method(rb_cGdkColor, "==", gdkcolor_equal, 1);
#if GTK_CHECK_VERSION(2, 12, 0)
    rb_define_method(rb_cGdkColor, "to_s", gdkcolor_to_string, 0);
#endif

    G_DEF_SETTERS(rb_cGdkColor);
}

