/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcurve.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:24 $

  Copyright (C) 2002,2003 The Ruby-GNOME2 Project
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
curve_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_curve_new());
    return Qnil;
}

static VALUE
curve_reset(self)
    VALUE self;
{
    gtk_curve_reset(GTK_CURVE(RVAL2GOBJ(self)));
    return self;
}

static VALUE
curve_set_gamma(self, gamma)
    VALUE self, gamma;
{
    gtk_curve_set_gamma(GTK_CURVE(RVAL2GOBJ(self)), NUM2DBL(gamma));
    return self;
}

static VALUE
curve_set_range(self, min_x, max_x, min_y, max_y)
    VALUE self, min_x, max_x, min_y, max_y;
{
    gtk_curve_set_range(GTK_CURVE(RVAL2GOBJ(self)),
			NUM2DBL(min_x), NUM2DBL(max_x),
			NUM2DBL(min_y), NUM2DBL(max_y));
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

    gtk_curve_set_vector(GTK_CURVE(RVAL2GOBJ(self)), len, c_vec);

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
    gtk_curve_get_vector(GTK_CURVE(RVAL2GOBJ(self)), len, c_vec);
    for (i = 0; i < len; i++)
      rb_ary_push(vector, rb_float_new(c_vec[i]));

    return vector;
}

void 
Init_gtk_curve()
{
    VALUE gCurve = G_DEF_CLASS(GTK_TYPE_CURVE, "Curve", mGtk);

    rb_define_method(gCurve, "initialize", curve_initialize, 0);
    rb_define_method(gCurve, "reset", curve_reset, 0);
    rb_define_method(gCurve, "set_gamma", curve_set_gamma, 1);
    rb_define_method(gCurve, "set_range", curve_set_range, 4);
    rb_define_method(gCurve, "set_vector", curve_set_vector, 2);
    rb_define_method(gCurve, "get_vector", curve_get_vector, 1);

    /* GtkCurveType(from standard constants) */
    rb_define_const(gCurve, "TYPE_LINEAR", INT2FIX(GTK_CURVE_TYPE_LINEAR));
    rb_define_const(gCurve, "TYPE_SPLINE", INT2FIX(GTK_CURVE_TYPE_SPLINE));
    rb_define_const(gCurve, "TYPE_FREE", INT2FIX(GTK_CURVE_TYPE_FREE));
}
