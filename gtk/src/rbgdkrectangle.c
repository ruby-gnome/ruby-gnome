/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkrectangle.c -

  $Author: mutoh $
  $Date: 2002/09/29 12:50:20 $

  Copyright (C) 2002 Masao Mutoh

  This file is derived from rbgdkregion.c.
  rbgdkregion.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(r) ((GdkRectangle*)RVAL2BOXED(r, GDK_TYPE_RECTANGLE))

static VALUE
gdkrect_initialize(self, x, y, width, height)
    VALUE self, x, y, width, height;
{
    GdkRectangle new;

    new.x = NUM2INT(x);
    new.y = NUM2INT(y);
    new.width = NUM2INT(width);
    new.height = NUM2INT(height);

    G_INITIALIZE(self, &new);
}

static VALUE
gdkrect_x(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->x);
}

static VALUE
gdkrect_y(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->y);
}

static VALUE
gdkrect_w(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
gdkrect_h(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
gdkrect_set_x(self, x)
    VALUE self, x;
{
    _SELF(self)->x = NUM2INT(x);
    return self;
}

static VALUE
gdkrect_set_y(self, y)
    VALUE self, y;
{
    _SELF(self)->y = NUM2INT(y);
    return self;
}

static VALUE
gdkrect_set_w(self, width)
    VALUE self, width;
{
    _SELF(self)->width = NUM2INT(width);
    return self;
}

static VALUE
gdkrect_set_h(self, height)
    VALUE self, height;
{
    _SELF(self)->height = NUM2INT(height);
    return self;
}

static VALUE
gdkrect_to_a(self)
    VALUE self;
{
  GdkRectangle* a = _SELF(self);
  return rb_ary_new3(4, INT2FIX(a->x), INT2FIX(a->y),
                     INT2FIX(a->width), INT2FIX(a->height));
}

void
Init_gtk_gdk_rectangle()
{
    VALUE gdkRectangle = G_DEF_CLASS(GDK_TYPE_RECTANGLE, "Rectangle", mGdk);

    rb_define_method(gdkRectangle, "initialize", gdkrect_initialize, 4);
    rb_define_method(gdkRectangle, "x", gdkrect_x, 0);
    rb_define_method(gdkRectangle, "y", gdkrect_y, 0);
    rb_define_method(gdkRectangle, "width", gdkrect_w, 0);
    rb_define_method(gdkRectangle, "height", gdkrect_h, 0);
    rb_define_method(gdkRectangle, "x=", gdkrect_set_x, 1);
    rb_define_method(gdkRectangle, "y=", gdkrect_set_y, 1);
    rb_define_method(gdkRectangle, "width=", gdkrect_set_w, 1);
    rb_define_method(gdkRectangle, "height=", gdkrect_set_h, 1);
    rb_define_method(gdkRectangle, "to_a", gdkrect_to_a, 0);
}
