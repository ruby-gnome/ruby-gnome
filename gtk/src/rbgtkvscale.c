/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkvscale.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vscale_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1;
    GtkAdjustment *adj = NULL;

    rb_scan_args(argc, argv, "01", &arg1);
    if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));

    RBGTK_INITIALIZE(self, gtk_vscale_new(adj));
    return Qnil;
}

void 
Init_gtk_vscale()
{
  VALUE gVScale = G_DEF_CLASS(GTK_TYPE_VSCALE, "VScale", mGtk);
  rb_define_method(gVScale, "initialize", vscale_initialize, -1);
}
