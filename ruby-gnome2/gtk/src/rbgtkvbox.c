/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvbox.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:35 $

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

    set_widget(self, gtk_vbox_new(RTEST(homogeneous),
				  (NIL_P(spacing)?0:NUM2INT(spacing))));
    return Qnil;
}

void Init_gtk_vbox()
{
    gVBox = rb_define_class_under(mGtk, "VBox", gBox);

    /* rb_define_const(gVBox, "SIGNAL_", rb_str_new2("")); */

    rb_define_method(gVBox, "initialize", vbox_initialize, -1);

    /* child initialization */
    Init_gtk_color_selection();
    Init_gtk_gamma_curve();
}
