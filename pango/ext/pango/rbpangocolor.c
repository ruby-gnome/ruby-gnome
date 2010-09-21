/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangocolor.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:11 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoColor*)RVAL2BOXED(self, PANGO_TYPE_COLOR))

static VALUE
color_initialize(self, red, green, blue)
    VALUE self, red, green, blue;
{
    PangoColor c;
    c.red = NUM2INT(red);
    c.green = NUM2INT(green);
    c.blue = NUM2INT(blue);

    G_INITIALIZE(self, &c);
    return Qnil;
}

static VALUE
color_parse(self, spec)
    VALUE self, spec;
{
    return CBOOL2RVAL(pango_color_parse(_SELF(self), RVAL2CSTR(spec)));
}

static VALUE
color_red(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->red);
}

static VALUE
color_set_red(self, red)
    VALUE self;
    VALUE red;
{
    _SELF(self)->red = NUM2INT(red);
    return self;
}

static VALUE
color_green(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->green);
}

static VALUE
color_set_green(self, green)
    VALUE self;
    VALUE green;
{
    _SELF(self)->green = NUM2INT(green);
    return self;
}

static VALUE
color_blue(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->blue);
}

static VALUE
color_set_blue(self, blue)
    VALUE self;
    VALUE blue;
{
    _SELF(self)->blue = NUM2INT(blue);
    return self;
}

static VALUE
color_to_a(self)
    VALUE self;
{
    PangoColor *c = _SELF(self);
    return rb_ary_new3(3, INT2FIX(c->red), 
                       INT2FIX(c->green), INT2FIX(c->blue));
}

static VALUE
color_equal(self, other)
	VALUE self, other;
{
    PangoColor* c1 = _SELF(self);
    PangoColor* c2 = _SELF(other);

    return CBOOL2RVAL((c1->red == c2->red) && (c1->green == c2->green) && 
                      (c1->blue == c2->blue));
}

void
Init_pango_color()
{
    VALUE pColor = G_DEF_CLASS(PANGO_TYPE_COLOR, "Color", mPango);
    
    rb_define_method(pColor, "initialize", color_initialize, 3);
    rb_define_method(pColor, "parse", color_parse, 1);
    rb_define_method(pColor, "red", color_red, 0);
    rb_define_method(pColor, "set_red", color_set_red, 1);
    rb_define_method(pColor, "green", color_green, 0);
    rb_define_method(pColor, "set_green", color_set_green, 1);
    rb_define_method(pColor, "blue", color_blue, 0);
    rb_define_method(pColor, "set_blue", color_set_blue, 1);
    rb_define_method(pColor, "to_a", color_to_a, 0);
    rb_define_method(pColor, "==", color_equal, 1);

    G_DEF_SETTERS(pColor);
}
