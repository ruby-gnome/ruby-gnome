/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvscale.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:08 $

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
  gVScale = rb_define_class_under(mGtk, "VScale", gScale);

  rb_define_method(gVScale, "initialize", vscale_initialize, -1);
}
