/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvruler.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vruler_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_vruler_new());
    return Qnil;
}

void 
Init_gtk_vruler()
{
    VALUE gVRuler = G_DEF_CLASS(GTK_TYPE_VRULER, "VRuler", mGtk);
    rb_define_method(gVRuler, "initialize", vruler_initialize, 0);
}
