/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkgamma.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

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

void Init_gtk_gamma_curve()
{
    static RGObjClassInfo cinfo;

    gGamma = rb_define_class_under(mGtk, "GammaCurve", gVBox);
    cinfo.klass = gGamma;
    cinfo.gtype = GTK_TYPE_GAMMA_CURVE;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gGamma, "initialize", gamma_initialize, 0);
    rb_define_method(gGamma, "gamma", gamma_gamma, 0);
    rb_define_method(gGamma, "curve", gamma_curve, 0);
}
