/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangocolor.c -

  $Author: mutoh $
  $Date: 2002/12/31 07:00:57 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
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


void
Init_pango_color()
{
    VALUE pColor = G_DEF_CLASS(PANGO_TYPE_COLOR, "Color", mPango);
    
    rb_define_method(pColor, "initialize", color_initialize, 3);
    rb_define_method(pColor, "parse", color_parse, 1);

}
