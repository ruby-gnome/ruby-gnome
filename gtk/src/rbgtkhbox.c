/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhbox.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:01 $

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

    RBGTK_INITIALIZE(self, gtk_hbox_new(RTEST(homogeneous),
				  (NIL_P(spacing)?0:NUM2INT(spacing))));
    return Qnil;
}

void 
Init_gtk_hbox()
{
    VALUE gHBox = G_DEF_CLASS(GTK_TYPE_HBOX, "HBox", mGtk);

    rb_define_method(gHBox, "initialize", hbox_initialize, -1);
}
