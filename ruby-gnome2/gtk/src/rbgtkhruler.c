/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhruler.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:35 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hruler_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_hruler_new());
    return Qnil;
}

void Init_gtk_hruler()
{
    gHRuler = rb_define_class_under(mGtk, "HRuler", gRuler);

    rb_define_method(gHRuler, "initialize", hruler_initialize, 0);
}
