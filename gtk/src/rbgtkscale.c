/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkscale.c -

  $Author: mutoh $
  $Date: 2002/05/19 13:59:10 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
scale_set_digits(self, digits)
    VALUE self, digits;
{
    gtk_scale_set_digits(GTK_SCALE(get_widget(self)), NUM2INT(digits));
    return self;
}

static VALUE
scale_set_draw_value(self, draw_value)
    VALUE self, draw_value;
{
    gtk_scale_set_draw_value(GTK_SCALE(get_widget(self)),
			     RTEST(draw_value));
    return self;
}

static VALUE
scale_set_value_pos(self, pos)
    VALUE self, pos;
{
    gtk_scale_set_value_pos(GTK_SCALE(get_widget(self)), 
			    (GtkPositionType)NUM2INT(pos));
    return self;
}

#if GTK_MAJOR_VERSION < 2

static VALUE
scale_value_width(self)
    VALUE self;
{
    int i = gtk_scale_value_width(GTK_SCALE(get_widget(self)));

    return INT2FIX(i);
}

static VALUE
scale_draw_value(self)
    VALUE self;
{
    gtk_scale_draw_value(GTK_SCALE(get_widget(self)));
    return self;
}

#endif

void Init_gtk_scale()
{
  gScale = rb_define_class_under(mGtk, "Scale", gRange);

  rb_define_method(gScale, "set_digits", scale_set_digits, 1);
  rb_define_method(gScale, "set_draw_value", scale_set_draw_value, 1);
  rb_define_method(gScale, "set_value_pos", scale_set_value_pos, 1);
#if GTK_MAJOR_VERSION < 2
  rb_define_method(gScale, "value_width", scale_value_width, 0);
  rb_define_method(gScale, "draw_value", scale_draw_value, 0);
#endif

  /* child init */
  Init_gtk_hscale();
  Init_gtk_vscale();
}
