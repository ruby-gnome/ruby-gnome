/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvseparator.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:07 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vsep_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_vseparator_new());
    return Qnil;
}

void Init_gtk_vseparator()
{
    gVSeparator = rb_define_class_under(mGtk, "VSeparator", gSeparator);

    rb_define_method(gVSeparator, "initialize", vsep_initialize, 0);
}
