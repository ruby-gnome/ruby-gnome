/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhscale.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

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
    if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(get_gobject(arg1));

    set_widget(self, gtk_hscale_new(adj));
    return Qnil;
}

void Init_gtk_hscale()
{
  static rbgtk_class_info cinfo;

  gHScale = rb_define_class_under(mGtk, "HScale", gScale);
  cinfo.klass = gHScale;
  cinfo.gtype = GTK_TYPE_HSCALE;
  cinfo.mark = 0;
  cinfo.free = 0;
  rbgtk_register_class(&cinfo);

  rb_define_method(gHScale, "initialize", hscale_initialize, -1);
}
