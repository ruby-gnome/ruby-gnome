/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvpaned.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:34 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vpaned_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_vpaned_new());
    return Qnil;
}

void Init_gtk_vpaned()
{
    gVPaned = rb_define_class_under(mGtk, "VPaned", gPaned);

    rb_define_method(gVPaned, "initialize", vpaned_initialize, 0);
}
