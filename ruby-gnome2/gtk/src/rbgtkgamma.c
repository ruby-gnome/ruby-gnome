/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkgamma.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:33 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
gamma_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_gamma_curve_new());
    return Qnil;
}

static VALUE
gamma_gamma(self)
    VALUE self;
{
    return rb_float_new(GTK_GAMMA_CURVE(get_widget(self))->gamma);
}

static VALUE
gamma_curve(self)
    VALUE self;
{
    return make_gobject(gCurve, GTK_OBJECT(GTK_GAMMA_CURVE(get_widget(self))->curve));
}

void Init_gtk_gamma_curve()
{
    gGamma = rb_define_class_under(mGtk, "GammaCurve", gVBox);

    rb_define_method(gGamma, "initialize", gamma_initialize, 0);
    rb_define_method(gGamma, "gamma", gamma_gamma, 0);
    rb_define_method(gGamma, "curve", gamma_curve, 0);
}
