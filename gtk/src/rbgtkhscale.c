/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhscale.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hscale_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1;
    GtkAdjustment *adj = NULL;

    rb_scan_args(argc, argv, "01", &arg1);
    if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));

    RBGTK_INITIALIZE(self, gtk_hscale_new(adj));
    return Qnil;
}

void 
Init_gtk_hscale()
{
  VALUE gHScale = G_DEF_CLASS(GTK_TYPE_HSCALE, "HScale", mGtk);

  rb_define_method(gHScale, "initialize", hscale_initialize, -1);
}
