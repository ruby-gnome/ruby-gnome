/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhseparator.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:37 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hsep_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_hseparator_new());
    return Qnil;
}

void Init_gtk_hseparator()
{
    gHSeparator = rb_define_class_under(mGtk, "HSeparator", gSeparator);

    rb_define_method(gHSeparator, "initialize", hsep_initialize, 0);
}
