/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvscale.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

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
    if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(get_gobject(arg1));

    set_widget(self, gtk_vscale_new(adj));
    return Qnil;
}

void Init_gtk_vscale()
{
  static rbgtk_class_info cinfo;

  gVScale = rb_define_class_under(mGtk, "VScale", gScale);
  cinfo.klass = gVScale;
  cinfo.gtype = GTK_TYPE_VSCALE;
  cinfo.mark = 0;
  cinfo.free = 0;
  rbgtk_register_class(&cinfo);

  rb_define_method(gVScale, "initialize", vscale_initialize, -1);
}
