/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkscale.c -

  $Author: sakai $
  $Date: 2002/08/01 17:46:18 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
scale_set_digits(self, digits)
    VALUE self, digits;
{
    gtk_scale_set_digits(GTK_SCALE(RVAL2GOBJ(self)), NUM2INT(digits));
    return self;
}

static VALUE
scale_set_draw_value(self, draw_value)
    VALUE self, draw_value;
{
    gtk_scale_set_draw_value(GTK_SCALE(RVAL2GOBJ(self)),
			     RTEST(draw_value));
    return self;
}

static VALUE
scale_set_value_pos(self, pos)
    VALUE self, pos;
{
    gtk_scale_set_value_pos(GTK_SCALE(RVAL2GOBJ(self)), 
			    (GtkPositionType)NUM2INT(pos));
    return self;
}

void 
Init_gtk_scale()
{
  VALUE gScale = G_DEF_CLASS(GTK_TYPE_SCALE, "Scale", mGtk);

  rb_define_method(gScale, "set_digits", scale_set_digits, 1);
  rb_define_method(gScale, "set_draw_value", scale_set_draw_value, 1);
  rb_define_method(gScale, "set_value_pos", scale_set_value_pos, 1);
}
