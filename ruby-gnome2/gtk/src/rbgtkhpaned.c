/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhpaned.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:39 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hpaned_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_hpaned_new());
    return Qnil;
}

void Init_gtk_hpaned()
{
    gHPaned = rb_define_class_under(mGtk, "HPaned", gPaned);

    rb_define_method(gHPaned, "initialize", hpaned_initialize, 0);
}
