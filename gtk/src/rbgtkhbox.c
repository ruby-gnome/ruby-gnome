/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhbox.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:03 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hbox_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE homogeneous, spacing;

    rb_scan_args(argc, argv, "02", &homogeneous, &spacing);

    set_widget(self, gtk_hbox_new(RTEST(homogeneous),
				  (NIL_P(spacing)?0:NUM2INT(spacing))));
    return Qnil;
}

void Init_gtk_hbox()
{
    gHBox = rb_define_class_under(mGtk, "HBox", gBox);

    /* rb_define_const(gHBox, "SIGNAL_", rb_str_new2("")); */

    rb_define_method(gHBox, "initialize", hbox_initialize, -1);

    /* child initialization */
    Init_gtk_combo();
    Init_gtk_statusbar();
}
