/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkscale.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

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

void Init_gtk_scale()
{
  static rbgtk_class_info cinfo;

  gScale = rb_define_class_under(mGtk, "Scale", gRange);
  cinfo.klass = gScale;
  cinfo.gtype = GTK_TYPE_SCALE;
  cinfo.mark = 0;
  cinfo.free = 0;
  rbgtk_register_class(&cinfo);

  rb_define_method(gScale, "set_digits", scale_set_digits, 1);
  rb_define_method(gScale, "set_draw_value", scale_set_draw_value, 1);
  rb_define_method(gScale, "set_value_pos", scale_set_value_pos, 1);

  /* child init */
  Init_gtk_hscale();
  Init_gtk_vscale();
}
