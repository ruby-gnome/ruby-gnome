/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvbox.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

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

void Init_gtk_vbox()
{
    static rbgtk_class_info cinfo;

    gVBox = rb_define_class_under(mGtk, "VBox", gBox);
    cinfo.klass = gVBox;
    cinfo.gtype = GTK_TYPE_VBOX;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    /* rb_define_const(gVBox, "SIGNAL_", rb_str_new2("")); */

    rb_define_method(gVBox, "initialize", vbox_initialize, -1);

    /* child initialization */
    Init_gtk_color_selection();
    Init_gtk_gamma_curve();
}
