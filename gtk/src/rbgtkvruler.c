/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvruler.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:09 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vruler_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_vruler_new());
    return Qnil;
}

void Init_gtk_vruler()
{
    gVRuler = rb_define_class_under(mGtk, "VRuler", gRuler);

    rb_define_method(gVRuler, "initialize", vruler_initialize, 0);
}
