/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhruler.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hruler_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_hruler_new());
    return Qnil;
}

void 
Init_gtk_hruler()
{
    VALUE gHRuler = G_DEF_CLASS(GTK_TYPE_HRULER, "HRuler", mGtk);

    rb_define_method(gHRuler, "initialize", hruler_initialize, 0);
}
