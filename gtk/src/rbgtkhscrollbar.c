/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhscrollbar.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hscrollbar_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1;
    GtkAdjustment *adj = NULL;

    rb_scan_args(argc, argv, "01", &arg1);
    if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));

    RBGTK_INITIALIZE(self, gtk_hscrollbar_new(adj));
    return Qnil;
}

void Init_gtk_hscrollbar()
{
  static rbgtk_class_info cinfo;

  gHScrollbar = rb_define_class_under(mGtk, "HScrollbar", gScrollbar);
  cinfo.klass = gHScrollbar;
  cinfo.gtype = GTK_TYPE_HSCROLLBAR;
  cinfo.mark = 0;
  cinfo.free = 0;
  rbgtk_register_class(&cinfo);

  rb_define_method(gHScrollbar, "initialize", hscrollbar_initialize, -1);
}
