/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvscale.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

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

void Init_gtk_vscale()
{
  static RGObjClassInfo cinfo;

  gVScale = rb_define_class_under(mGtk, "VScale", gScale);
  cinfo.klass = gVScale;
  cinfo.gtype = GTK_TYPE_VSCALE;
  cinfo.mark = 0;
  cinfo.free = 0;
  rbgtk_register_class(&cinfo);

  rb_define_method(gVScale, "initialize", vscale_initialize, -1);
}
