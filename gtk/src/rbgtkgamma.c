/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkgamma.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:23 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
gamma_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_gamma_curve_new());
    return Qnil;
}

static VALUE
gamma_gamma(self)
    VALUE self;
{
    return rb_float_new(GTK_GAMMA_CURVE(RVAL2GOBJ(self))->gamma);
}

static VALUE
gamma_curve(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_GAMMA_CURVE(RVAL2GOBJ(self))->curve);
}

void 
Init_gtk_gamma_curve()
{
    VALUE gGamma = G_DEF_CLASS(GTK_TYPE_GAMMA_CURVE, "GammaCurve", mGtk);

    rb_define_method(gGamma, "initialize", gamma_initialize, 0);
    rb_define_method(gGamma, "gamma", gamma_gamma, 0);
    rb_define_method(gGamma, "curve", gamma_curve, 0);
}
