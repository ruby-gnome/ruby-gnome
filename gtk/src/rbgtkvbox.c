/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvbox.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vbox_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE homogeneous, spacing;

    rb_scan_args(argc, argv, "02", &homogeneous, &spacing);

    RBGTK_INITIALIZE(self, gtk_vbox_new(RTEST(homogeneous),
				  (NIL_P(spacing)?0:NUM2INT(spacing))));
    return Qnil;
}

void 
Init_gtk_vbox()
{
    VALUE gVBox = G_DEF_CLASS(GTK_TYPE_VBOX, "VBox", mGtk);

    rb_define_method(gVBox, "initialize", vbox_initialize, -1);

    /* child initialization */
    Init_gtk_color_selection();
    Init_gtk_gamma_curve();
}
