/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkcurve.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:09 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
curve_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_curve_new());
    return Qnil;
}

static VALUE
curve_reset(self)
    VALUE self;
{
    gtk_curve_reset(GTK_CURVE(get_widget(self)));
    return self;
}

static VALUE
curve_set_gamma(self, gamma)
    VALUE self, gamma;
{
    gtk_curve_set_gamma(GTK_CURVE(get_widget(self)), NUM2DBL(gamma));
    return self;
}

static VALUE
curve_set_range(self, min_x, max_x, min_y, max_y)
    VALUE self, min_x, max_x, min_y, max_y;
{
    gtk_curve_set_range(GTK_CURVE(get_widget(self)),
			NUM2DBL(min_x), NUM2DBL(max_x),
			NUM2DBL(min_y), NUM2DBL(max_y));
    return self;
}

static VALUE
curve_set_curve_type(self, type)
    VALUE self, type;
{
    gtk_curve_set_curve_type(GTK_CURVE(get_widget(self)), NUM2INT(type));
    return self;
}

static VALUE
curve_set_vector(self, length, vector)
    VALUE self, length, vector;
{
    gint len = NUM2INT(length);
    gfloat *c_vec;
    gint i;

    c_vec = ALLOCA_N(gfloat, len);
    for (i = 0; i < len; i++)
      c_vec[i] = NUM2DBL(RARRAY(vector)->ptr[i]);
    gtk_curve_set_vector(GTK_CURVE(get_widget(self)), len, c_vec);
    return self;
}

static VALUE
curve_get_vector(self, length)
    VALUE self, length;
{
    gint len = NUM2INT(length);
    gfloat *c_vec;
    gint i;
    VALUE vector = rb_ary_new2(len);

    c_vec = ALLOCA_N(gfloat, len);
    gtk_curve_get_vector(GTK_CURVE(get_widget(self)), len, c_vec);
    for (i = 0; i < len; i++)
      rb_ary_push(vector, rb_float_new(c_vec[i]));
    return vector;
}

void Init_gtk_curve()
{
    gCurve = rb_define_class_under(mGtk, "Curve", gDrawArea);

    rb_define_const(gCurve, "SIGNAL_CURVE_TYPE_CHANGED", rb_str_new2("curve-type-changed"));

    rb_define_method(gCurve, "initialize", curve_initialize, 0);
    rb_define_method(gCurve, "reset", curve_reset, 0);
    rb_define_method(gCurve, "set_gamma", curve_set_gamma, 1);
    rb_define_method(gCurve, "set_range", curve_set_range, 4);
    rb_define_method(gCurve, "set_curve_type", curve_set_curve_type, 1);
    rb_define_method(gCurve, "set_vector", curve_set_vector, 2);
    rb_define_method(gCurve, "get_vector", curve_get_vector, 1);

    rb_define_const(gCurve, "CURVE_TYPE_LINEAR", INT2FIX(GTK_CURVE_TYPE_LINEAR));
    rb_define_const(gCurve, "CURVE_TYPE_SPLINE", INT2FIX(GTK_CURVE_TYPE_SPLINE));
    rb_define_const(gCurve, "CURVE_TYPE_FREE", INT2FIX(GTK_CURVE_TYPE_FREE));
}
