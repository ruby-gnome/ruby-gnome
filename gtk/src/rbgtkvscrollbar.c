/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvscrollbar.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vscrollbar_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1;
    GtkAdjustment *adj = NULL;

    rb_scan_args(argc, argv, "01", &arg1);
    if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));

    RBGTK_INITIALIZE(self, gtk_vscrollbar_new(adj));
    return Qnil;
}

void Init_gtk_vscrollbar()
{
  static RGObjClassInfo cinfo;

  gVScrollbar = rb_define_class_under(mGtk, "VScrollbar", gScrollbar);
  cinfo.klass = gVScrollbar;
  cinfo.gtype = GTK_TYPE_VSCROLLBAR;
  cinfo.mark = 0;
  cinfo.free = 0;
  rbgtk_register_class(&cinfo);
  
  rb_define_method(gVScrollbar, "initialize", vscrollbar_initialize, -1);
}
